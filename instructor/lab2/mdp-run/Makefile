N=08
M=16
K=2
SEED=1
TLIM=600

INTERP = python3
RM=rm -f

GDIR = ../generators
GEN = $(GDIR)/mdparity.py
MCHECK = $(GDIR)/mdpcheck.py

TDIR = ../../../tools

KISSAT = $(TDIR)/kissat/build/kissat
TBSAT  = $(TDIR)/tbuddy/bin/tbsat
EXTRACT = $(TDIR)/tbuddy/tools/xor_extractor.py

DRAT = $(TDIR)/drat-trim/drat-trim
LRAT = $(TDIR)/drat-trim/lrat-check


cnf: mdpx-n$(N)-m$(M)-k$(K)-s$(SEED).cnf

knp: mdpx-n$(N)-m$(M)-k$(K)-s$(SEED).kissat_noproof_data

bnp: mdpx-n$(N)-m$(M)-k$(K)-s$(SEED).bucket_noproof_data

gnp: mdpx-n$(N)-m$(M)-k$(K)-s$(SEED).gauss_noproof_data

kp: mdpx-n$(N)-m$(M)-k$(K)-s$(SEED).kissat_proof_data

bp: mdpx-n$(N)-m$(M)-k$(K)-s$(SEED).bucket_proof_data

gp: mdpx-n$(N)-m$(M)-k$(K)-s$(SEED).gauss_proof_data

mdpx-n$(N)-m$(M)-k$(K)-s$(SEED).cnf: $(GEN)
	$(INTERP) $(GEN) -x -p -n $(N) -m $(M) -k $(K) -s $(SEED) -r mdpx-n$(N)-m$(M)-k$(K)-s$(SEED)

.SUFFIXES: .cnf .kissat_noproof_data .kissat_proof_data .bucket_noproof_data .bucket_proof_data .gauss_noproof_data .gauss_proof_data

.cnf.kissat_noproof_data:
	$(KISSAT) --time=$(TLIM) $< | tee $@
	$(INTERP) $(MCHECK) $< $@ | tee -a $@

.cnf.bucket_noproof_data:
	$(TBSAT) -t $(TLIM) -b -i $< -p $*.order | tee $@
	$(INTERP) $(MCHECK) $< $@ | tee -a $@

.cnf.gauss_noproof_data:
	$(INTERP) $(EXTRACT) -i $< -o $*.schedule | tee $@
	$(TBSAT) -t $(TLIM) -i $< -p $*.order -s $*.schedule | tee -a $@
	$(INTERP) $(MCHECK) $< $@ | tee -a $@

.cnf.kissat_proof_data:
	$(KISSAT) --time=$(TLIM) $< $*.drat | tee $@
	$(DRAT) $< $*.drat | tee -a $@
	$(RM) $*.drat

.cnf.bucket_proof_data:
	$(TBSAT) -t $(TLIM) -b -i $< -p $*.order -o $*.lrat | tee $@
	$(LRAT) $< $*.lrat | tee -a $@
	$(RM) $*.lrat

.cnf.gauss_proof_data:
	$(INTERP) $(EXTRACT) -i $< -o $*.schedule  | tee $@
	$(TBSAT) -t $(TLIM) -i $< -p $*.order -s $*.schedule -o $*.lrat | tee -a $@
	$(LRAT) $< $*.lrat | tee -a $@
	$(RM) $*.lrat

clean:
	rm -f *.cnf *.drat *.lrat *.order *.schedule



