# ====== Output Files ======
ADMIN_OUT = Admin/adminControls.out
CHALLAN_OUT = Challans/challan.out
PSEUDO_OUT = pseudo_echallan.out
REG_OUT = Registrations/registration.out
BANK_OUT = Finances/bank.out

# ====== Common Sources ======
COMMON = Functions/functions.cpp

# ====== Admin ======
admin: $(ADMIN_OUT)

$(ADMIN_OUT): Admin/adminControls.cpp $(COMMON)
	g++ Admin/adminControls.cpp $(COMMON) -o $(ADMIN_OUT) `pkg-config --cflags --libs opencv4`

run-admin: admin
	./$(ADMIN_OUT)

# ====== Challan ======
challan: $(CHALLAN_OUT)

$(CHALLAN_OUT): Challans/challan.cpp $(COMMON)
	g++ Challans/challan.cpp $(COMMON) -o $(CHALLAN_OUT) `pkg-config --cflags --libs opencv4`

run-challan: challan
	./$(CHALLAN_OUT)

# ====== Pseudo E-Challan ======
pseudo: $(PSEUDO_OUT)

$(PSEUDO_OUT): pseudo_echallan.cpp $(COMMON)
	g++ pseudo_echallan.cpp $(COMMON) -o $(PSEUDO_OUT) `pkg-config --cflags --libs opencv4`

run-pseudo: pseudo
	./$(PSEUDO_OUT)

# ====== Registration ======
registration: $(REG_OUT)

$(REG_OUT): Registrations/registration.cpp $(COMMON)
	g++ Registrations/registration.cpp $(COMMON) -o $(REG_OUT) `pkg-config --cflags --libs opencv4`

run-registration: registration
	./$(REG_OUT)

# ====== Bank System ======
bank: $(BANK_OUT)

$(BANK_OUT): Finances/bank.cpp $(COMMON)
	g++ Finances/bank.cpp $(COMMON) -o $(BANK_OUT) `pkg-config --cflags --libs opencv4`

run-bank: bank
	./$(BANK_OUT)

# ====== Build Everything ======
all: admin challan pseudo registration bank

# ====== Clean ======
clean:
	rm -f $(ADMIN_OUT) $(CHALLAN_OUT) $(PSEUDO_OUT) $(REG_OUT) $(BANK_OUT)