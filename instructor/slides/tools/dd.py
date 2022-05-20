# Support generation of decision diagram images

import math

# Process files containing basic drawing commands.
# Each command has combination of positional and keyword arguments

# DDs drawn on grid with (0,0) at lower, lefthand corner
# Nominal distance between two levels = 100
# Default radius of branch or leaf node is 25.  Root node=5

# General rule:
#   Only need to give as much of a keyword or value as required to disambiguate it

# Commands
# Node
#    n ID X Y label=TTT type=branch radius=25/5 fill=background edge=neutral [only=XX]
#       ID is unique alphanumeric identifier
#       TTT is something in TeX (with no spaces)
#       type options: branch, root, leaf, empty


# Tag
#    t ID TTT position=above offset=10 [only=XX]
#       ID identifies node to tag
#       TTT is something in TeX.
#       Position options: above, below, left, right

# Edge
#    e FID TID value=neutral mark=none [only=XX]
#       FID and TID are Ids of source and destination nodes
#       value options: neutral, high, low, path
#       mark options: none, bubble   

# Graphic primitives
#
# Rectangle
#    r LLX LLY URX URY options=""  [only=XX]
#       Options can be any text enclosed in square brackets in Tikz.  Use ~ instead of space

# Colors
#    Either alter existing color definition or define new one
#    Existing (prefixes are sufficient):
#      neutralcolor: outline for nodes, default for edges, fill for roots
#      highcolor: High edge
#      lowcolor: Low edge
#      pathcolor: User-defined
#      fillcolor: Fill in leaf & branch nodes
#      backgroundcolor: Fill for rectangles
#    VALUE: R:G:B values (each integer from 0 to 255)

def trim(s):
    while len(s) > 0 and s[-1] == '\n':
        s = s[:-1]
    return s

class NoMatchException(Exception):
    
    prefix = ""

    def __init__(self, prefix):
        self.prefix = prefix

    def __str__(self):
        msg = "Match exception.  No keyword matching prefix '%s'" % self.prefix
        return msg

class MultiMatchException(Exception):

    prefix = ""
    def __init__(self, prefix):
        self.prefix = prefix

    def __str__(self):
        msg = "Match exception.  Multiple keywords matching prefix '%s'" % self.prefix
        return msg
    

# Given list of possible values for a label, determine if there is a unique
# match to prefix.  If so, return that prefix.  Otherwise, raise exception
def parseLabel(prefix, candidates, errorOK = False):
    clist = list(candidates)
    ccount = len(candidates)
    lastMatch = clist[-1]
    for cpos in range(len(prefix)):
        ccount = 0
        lastMatch = None
        for idx in range(len(clist)):
            label = clist[idx]
            if label is None:
                continue
            if cpos >= len(label) or label[cpos] != prefix[cpos]:
                clist[idx] = None
            else:
                ccount += 1
                lastMatch = label
        if ccount == 0:
            if errorOK:
                return None
            raise NoMatchException(prefix)
        elif ccount == 1 and cpos == len(prefix)-1:
            return lastMatch
    if ccount == 1:
        return lastMatch
    if errorOK:
        return None
    raise MultiMatchException(prefix)
                            
def getIndex(prefix, candidates):
    label = parseLabel(prefix, candidates)
    for idx in range(len(candidates)):
        if candidates[idx] == label:
            return idx
    # Shouldn't get here
    return None

class ParseException(Exception):
    line = ""
    info = ""

    def __init__(self, line, info):
        self.line = line
        self.info = info

    def __str__(self):
        msg = "Parse error.  Invalid line '%s' (%s)" % (self.line, self.info)
        return msg
        
class Scheme:
    blackWhite, color = list(range(2))
    names = ["bw", "color"]
    bwRGB = {
        "fillcolor" : (255,255,255), # White
        "highcolor" : (0,0,0),       # Black        
        "lowcolor"  : (0,0,0),       # Black        
        "neutralcolor"  : (0,0,0),       # Black
        "pathcolor"  : (0,0,0),       # Black
        # Provided to support special effects
        "background" : (225,225,225), # Light gray
    }
    colorRGB = {
        "fillcolor" : (255,255,224), # Light yellow
        "highcolor" : (204,0,0),     # Darkish red
        "lowcolor"  : (0,143,0),     # Darkish green
        "neutralcolor"  : (0,0,102),       # Dark blue
        "pathcolor"  : (25,25,255),       # Bright blue
        "background" : (230,240,255), # Very light blue
    }
    newColors = {}
    lineThickness = ["thin", "ultra thick"]

    def __init__(self):
        self.newColors = {}

    def parse(self, prefix):
        return getIndex(prefix, self.names)

    def declare(self, outfile, scheme):
        cdict = self.bwRGB if scheme == self.blackWhite else self.colorRGB
        for k in cdict.keys():
            if k in self.newColors:
                pass
            R,G,B = cdict[k]
            outfile.write("\\definecolor{%s}{RGB}{%d,%d,%d}\n" % (k, R, G, B))
        for k in self.newColors.keys():
            R,G,B = self.newColors[k]
            outfile.write("\\definecolor{%s}{RGB}{%d,%d,%d}\n" % (k, R, G, B))

    # Process color command
    def parseCommand(self, line):
        fields = line.split()
        if len(fields) < 2:
            raise ParseException(line, "Not enough fields")
        prefix = fields[0]
        candidates = self.colorRGB.keys()
        value = fields[1]
        name = parseLabel(prefix, candidates, errorOK = True)
        if name is None:
            name = prefix
        vfields = value.split(':')
        colorValue = None
        if len(vfields) == 3:
            try:
                R = int(vfields[0])
                G = int(vfields[1])
                B = int(vfields[2])
                colorValue = (R,G,B)
            except:
                pass
        if colorValue is None:
            raise ParseException(line, "Couldn't get RGB values from '%s'" % value)
        self.newColors[name] = colorValue

# Category of objects that can be guarded by \only specifier
class Only:
    only = None
    indent = ""

    def __init__(self):
        self.only = None
        self.indent = ""

    def setOnly(self, value):
        if len(value) > 0:
            if value[0] != '<':
                value = '<' + value
            if value[-1] != '>':
                value += '>'
            self.only = value

    def onlyPrefix(self, outfile):
        if self.only is not None:
            outfile.write("\\only%s{\n" % self.only)
            self.indent = "  "

    def onlySuffix(self, outfile):
        if self.only is not None:
            outfile.write("}\n")
            self.indent = ""


class NodeType:
    branch, leaf, root, empty = list(range(4))
    names = ["branch", "leaf", "root", "empty"]
    # Default radii
    radius = [25,25,5,0]

    def parse(self, prefix):
        return getIndex(prefix, self.names)


class Node(Only):
    id = None
    x = 0.0
    y = 0.0
    label = ""
    ntype = None
    radius = None
    only = None
    indent = ""
    fill=None
    edge=None

    def __init__(self, id=None, x=0.0, y=0.0, label="", ntype = None, radius = None):
        Only.__init__(self)
        self.id = id
        self.x = x
        self.y = y
        self.label = label
        self.ntype = NodeType.branch if ntype is None else ntype
        self.radius = self.defaultRadius(self.ntype) if radius is None else radius
        self.only = None
        self.indent = ""

    def defaultRadius(self, ntype):
        return NodeType.radius[ntype]

    def parse(self, line):
        fields = line.split()
        if len(fields) < 3:
            raise ParseException(line, "Not enough fields")
        self.id = fields[0]
        try:
            self.x = int(fields[1])
        except:
            raise ParseException(line, "Invalid x value")
        try:
            self.y = int(fields[2])
        except:
            raise ParseException(line, "Invalid y value")            
        setRadius = False
        for field in fields[3:]:
            info = field.split("=")
            if len(info) < 2:
                raise ParseException(line, "Invalid key=value syntax")
            prefix = info[0]
            value = '='.join(info[1:])
            try:
                key = parseLabel(prefix, ["label", "type", "radius", "only", "fill", "edge"])
            except NoMatchException:
                raise ParseException(line, "Invalid prefix '%s'" % prefix)
            except MultiMatchException:
                raise ParseException(line, "Ambiguous prefix '%s'" % prefix)
            if key == "label":
                self.label = value
            elif key == "radius":
                try:
                    self.radius = int(value)
                    setRadius = True
                except:
                    raise ParseException(line, "Invalid radius")
            elif key == "only":
                self.setOnly(value)
            elif key == "fill":
                self.fill = value
            elif key == "edge":
                self.edge = value
            else:
                try:
                    self.ntype = NodeType().parse(value)
                except NoMatchException:
                    raise ParseException(line, "Invalid type '%s'" % value)
                except MultiMatchException:
                    raise ParseException(line, "Ambiguous type '%s'" % value)
        if not setRadius:
            self.radius = self.defaultRadius(self.ntype)
        return self
        
    def renderTikz(self, outfile, lowerLeft = (0,0), scale=1.0, scheme=Scheme.blackWhite):
        llx, lly = lowerLeft
        cx = scale * (self.x - llx)
        cy = scale * (self.y - lly)
        fill = self.fill if self.fill is not None else "neutralcolor" if self.ntype == NodeType.root else "fillcolor"
        line = Scheme.lineThickness[scheme]
        edge = self.edge if self.edge is not None else "neutralcolor"
        self.onlyPrefix(outfile)
        if self.ntype == NodeType.leaf:
            lx = scale * (self.x - llx - self.radius)
            ly = scale * (self.y - lly - self.radius)
            ux = scale * (self.x - llx + self.radius)
            uy = scale * (self.y - lly + self.radius)
            outfile.write("%s\\draw [%s,fill=%s,draw=%s] (%.2f,%.2f) rectangle (%.2f,%.2f);\n" % (self.indent, line, fill, edge, lx,ly,ux,uy))
        elif self.ntype in  [NodeType.branch, NodeType.root]:
            r = scale * self.radius
            outfile.write("%s\\draw [%s,fill=%s,draw=%s] (%.2f,%.2f) circle [radius=%.2f];\n" % (self.indent, line, fill, edge, cx,cy,r))
        if self.label is not None:
            outfile.write("%s\\node at (%.2f,%.2f) {%s};\n" % (self.indent, cx,cy,self.label))
        self.onlySuffix(outfile)

            
class EdgeType:
    neutral, high, low, path = list(range(4))
    names = ["neutral", "high", "low", "path"]
    none, bubble = list(range(2))
    markNames = ["none", "bubble"]

    def parse(self, prefix):
        return getIndex(prefix, self.names)
        
    def parseMark(self, prefix):
        return getIndex(prefix, self.markNames)

class Edge(Only):
    fromId = None
    fromNode = None
    toId = None
    toNode = None
    etype = None
    mtype = EdgeType.none
    edgeSpacing = 12.0
    edgeFraction = 0.65

    def __init__(self, fromId=None, toId=None, etype=None):
        Only.__init__(self)
        self.fromId = fromId
        self.fromNode = None
        self.toId = toId
        self.toNode = None
        self.etype = EdgeType.neutral if etype is None else etype

    def parse(self, line):
        fields = line.split()
        if len(fields) < 2:
            raise ParseException(line, "Not enough fields")
        self.fromId = fields[0]
        self.toId = fields[1]
        for field in fields[2:]:
            info = field.split("=")
            if len(info) != 2:
                raise ParseException(line, "Invalid key=value syntax")
            prefix, value = info
            try:
                key = parseLabel(prefix, ["type", "mark", "only"])
            except NoMatchException:
                raise ParseException(line, "Invalid prefix '%s'" % prefix)
            except MultiMatchException:
                raise ParseException(line, "Ambiguous prefix '%s'" % prefix)
            if key == "type":
                try:
                    self.etype = EdgeType().parse(value)
                except NoMatchException:
                    raise ParseException(line, "Invalid type '%s'" % value)
                except MultiMatchException:
                    raise ParseException(line, "Ambiguous type '%s'" % value)
            elif key == "mark":
                try:
                    self.mtype = EdgeType().parseMark(value)
                except NoMatchException:
                    raise ParseException(line, "Invalid mark '%s'" % value)
                except MultiMatchException:
                    raise ParseException(line, "Ambiguous mark '%s'" % value)
            elif key == "only":
                self.setOnly(value)
        return self
        
    def renderTikz(self, outfile, lowerLeft, scale=1.0, scheme=Scheme.blackWhite):
        snode = self.fromNode
        fnode = self.toNode
        llx, lly = lowerLeft
        sx = scale * (snode.x - llx)
        sy = scale * (snode.y - lly)
        fx = scale * (fnode.x - llx)
        fy = scale * (fnode.y - lly)
        edge = "neutralcolor"
        line = Scheme.lineThickness[scheme]
        if self.etype == EdgeType.low:
            edge = "lowcolor"
        elif self.etype == EdgeType.high:
            edge = "highcolor"
        elif self.etype == EdgeType.path:
            edge = "pathcolor"
        self.onlyPrefix(outfile)
        if self.etype == EdgeType.low:
            outfile.write("%s\\draw [%s,%s,dashed] (%.2f,%.2f) -- (%.2f,%.2f);\n" % (self.indent, line, edge, sx,sy,fx,fy))
        else:
            outfile.write("%s\\draw (%.2f,%.2f) [%s,%s] -- (%.2f,%.2f);\n" % (self.indent, sx,sy,line,edge,fx,fy))
        if self.mtype == EdgeType.bubble:
            cx = (sx+fx)/2
            cy = (sy+fy)/2
            r = scale * NodeType.radius[NodeType.root]
            outfile.write("%s\\draw [fill=%s,draw=%s] (%.2f,%.2f) circle [radius=%.2f];\n" % (self.indent, edge, edge, cx,cy,r))
        self.onlySuffix(outfile)

        
class PositionType:
    above, below, left, right = list(range(4))
    names = ["above", "below", "left", "right"]

    def parse(self, prefix):
        return getIndex(prefix, self.names)
    

class Tag(Only):
    id = None
    node = None
    text = ""
    position = None
    offset= None
    
    def __init__(self, id=None, node=None, text="", position=None, offset=10):
        Only.__init__(self)
        self.id = id
        self.node = node
        self.text = text
        self.position = PositionType.above if position is None else position
        self.offset = offset

    def parse(self, line):
        fields = line.split()
        if len(fields) < 2:
            raise ParseException(line, "Not enough fields")
        self.id = fields[0]
        self.text = fields[1]
        for field in fields[2:]:
            info = field.split("=")
            if len(info) != 2:
                raise ParseException(line, "Invalid key=value syntax")
            prefix, value = info
            try:
                key = parseLabel(prefix, ["position", "offset", "only"])
            except NoMatchException:
                raise ParseException(line, "Invalid prefix '%s'" % prefix)
            except MultiMatchException:
                raise ParseException(line, "Ambiguous prefix '%s'" % prefix)
            if key == "position":
                try:
                    self.position = PositionType().parse(value)
                except NoMatchException:
                    raise ParseException(line, "Invalid position '%s'" % value)
                except MultiMatchException:
                    raise ParseException(line, "Ambiguous position '%s'" % value)
            elif key == "offset":
                try:
                    self.offset = int(value)
                except:
                    raise ParseException(line, "Invalid offset value '%s'" % value)
            elif key == "only":
                self.setOnly(value)

        return self

    def renderTikz(self, outfile, lowerLeft = (0,0), scale=1.0, scheme=Scheme.blackWhite):
        llx, lly = lowerLeft
        px = scale * (self.node.x - llx)
        py = scale * (self.node.y - lly)
        delta = scale * (self.node.radius + self.offset)
        if self.position == PositionType.above:
            py += delta
        elif self.position == PositionType.below:
            py -= delta
        elif self.position == PositionType.left:
            px -= delta
        elif self.position == PositionType.right:
            px += delta
        pstring = PositionType.names[self.position]
        self.onlyPrefix(outfile)
        if self.text is not None:
            outfile.write("%s\\node [%s] at (%.2f,%.2f) {%s};\n" % (self.indent, pstring, px,py,self.text))
        self.onlySuffix(outfile)

class Rectangle(Only):
    llx = 0.0
    lly = 0.0
    urx = 0.0
    ury = 0.0
    options = None

    def __init__(self):
        Only.__init__(self)
        self.options = None

    def parse(self, line):
        self.options = None
        fields = line.split()
        if len(fields) < 4:
            raise ParseException(line, "Not enough fields")
        try:
            self.llx = int(fields[0])
            self.lly = int(fields[1])
            self.urx = int(fields[2])
            self.ury = int(fields[3])
        except:
            raise ParseException(line, "Invalid coordinate")
        try:
            self.y = int(fields[2])
        except:
            raise ParseException(line, "Invalid y value")            
        setRadius = False
        for field in fields[4:]:
            info = field.split("=")
            if len(info) < 2:
                raise ParseException(line, "Invalid key=value syntax")
            prefix = info[0]
            value = "=".join(info[1:])
            try:
                key = parseLabel(prefix, ["options", "only"])
            except NoMatchException:
                raise ParseException(line, "Invalid prefix '%s'" % prefix)
            except MultiMatchException:
                raise ParseException(line, "Ambiguous prefix '%s'" % prefix)
            if key == "options":
                vfields = value.split('~')
                self.options = ' '.join(vfields)
            elif key == "only":
                self.setOnly(value)
        return self
        
    def renderTikz(self, outfile, lowerLeft = (0,0), scale=1.0, scheme=Scheme.blackWhite):
        llx = scale * (self.llx - lowerLeft[0])
        lly = scale * (self.lly - lowerLeft[1])
        urx = scale * (self.urx - lowerLeft[0])
        ury = scale * (self.ury - lowerLeft[1])
        if self.options is not None:
            options = self.options
            if options[0] != '[':
                options = '[' + options + ']'
        self.onlyPrefix(outfile)
        outfile.write("%s\\draw %s (%.2f,%.2f) rectangle (%.2f,%.2f);\n" % (self.indent, options, llx,lly,urx,ury))
        self.onlySuffix(outfile)

class GraphException(Exception):
    msg = ""

    def __init__(self, msg):
        self.msg = msg

    def __str__(self):
        info = "Graph error. (%s)" % (self.msg)
        return info


class Graph:
    # Set of nodes, indexed by their IDs
    nodeDict = {}
    # List of edges rendered in background
    edgeList = []
    # List of edges rendered in foreground
    foreEdgeList = []
    # List of tags
    tagList = []
    # List of rectangles
    rectangleList = []
    # Color scheme
    scheme = None

    def __init__(self):
        self.nodeDict = {}
        self.edgeList = []
        self.foreEdgeList = []
        self.tagList = []
        self.rectangleList = []
        self.scheme = Scheme()

    def addNode(self, nd):
        if nd.id in self.nodeDict:
            raise GraphException("Graph already has node named '%s'" % nd.id)
        self.nodeDict[nd.id] = nd

    def addEdge(self, edge):
        if edge.fromId not in self.nodeDict:
            raise GraphException("Graph has no node named '%s'" % edge.fromId)
        edge.fromNode = self.nodeDict[edge.fromId]
        if edge.toId not in self.nodeDict:
            raise GraphException("Graph has no node named '%s'" % edge.toId)
        edge.toNode = self.nodeDict[edge.toId]
        if edge.etype == EdgeType.path:
            self.foreEdgeList.append(edge)
        else:
            self.edgeList.append(edge)

    def addTag(self, tag):
        if tag.id not in self.nodeDict:
            raise GraphException("Graph has no node named '%s'" % tag.id)
        tag.node = self.nodeDict[tag.id]
        self.tagList.append(tag)

    def addRectangle(self, rect):
        self.rectangleList.append(rect)

    def boundingBox(self):
        xmin = None
        xmax = None
        ymin = None
        ymax = None
        for nd in self.nodeDict.values():
            nxmin, nxmax = nd.x - nd.radius, nd.x + nd.radius
            nymin, nymax = nd.y - nd.radius, nd.y + nd.radius
            if xmin is None or xmin > nxmin:
                xmin = nxmin
            if xmax is None or xmax < nxmax:
                xmax = nxmax
            if ymin is None or ymin > nymin:
                ymin = nymin
            if ymax is None or ymax < nymax:
                ymax = nymax
        return ((xmin, ymin), (xmax,ymax))

    def load(self, infile):
        lineNumber = 0
        for line in infile:
            line = trim(line)
            lineNumber += 1
            fields = line.split()
            if len(fields) == 0:
                continue
            elif fields[0][0] == '#':
                continue
            cmd = fields[0]
            rest = " ".join(fields[1:])
            if cmd == 'n':
                nd = Node().parse(rest)
                self.addNode(nd)
            elif cmd == 'e':
                edge = Edge().parse(rest)
                self.addEdge(edge)
            elif cmd == 't':
                tag = Tag().parse(rest)
                self.addTag(tag)
            elif cmd == 'r':
                rect = Rectangle().parse(rest)
                self.addRectangle(rect)
            elif cmd == 'c':
                self.scheme.parseCommand(rest)
            else:
                raise ParseException(line, "Unknown command")
            
    # Tikz uses centimeters as base unit
    # Program specifies desired length per 100 x/y units
    def findScale(self, unitLength):
        unitConversions = { 'cm' : 1.0 , 'mm' : 0.1 , 'in' : 2.54, 'pt' : 0.0353 }
        # See how to scale to centimeters
        scale = 1.0
        units = unitLength[-2:]
        try:
            uval = float(unitLength[:-2])
        except:
            print("Warning: Could not parse unit length '%s'" % unitLength)
            return scale
        if units in unitConversions:
            scale = 0.01 * uval * unitConversions[units]
        else:
            print("Unknown units '%s'" % units)
        return scale

    def renderTikz(self, outfile, unitLength='50pt', scheme=Scheme.blackWhite):
        scale = self.findScale(unitLength)
        lowerLeft, upperRight = self.boundingBox()
        outfile.write("\\begin{tikzpicture}\n")
        self.scheme.declare(outfile, scheme)
        for rect in self.rectangleList:
            rect.renderTikz(outfile, lowerLeft, scale, scheme)
        for edge in self.edgeList:
            edge.renderTikz(outfile, lowerLeft, scale, scheme)
        for nd in self.nodeDict.values():
            nd.renderTikz(outfile, lowerLeft, scale, scheme)
        for edge in self.foreEdgeList:
            edge.renderTikz(outfile, lowerLeft, scale, scheme)
        for tag in self.tagList:
            tag.renderTikz(outfile, lowerLeft, scale, scheme)
        outfile.write("\\end{tikzpicture}%%\n")
        
