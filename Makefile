admin: Admin/adminControls.cpp
	g++ Admin/adminControls.cpp Functions/functions.cpp -o Admin/adminControls `pkg-config --cflags --libs opencv4`

run-admin: admin
	./Admin/adminControls

challan: Challans/challan.cpp
	g++ Challans/challan.cpp Functions/functions.cpp -o Challans/challan `pkg-config --cflags --libs opencv4`

run-challan: challan
	./Challans/challan


pseudo_echallan: pseudo_echallan.cpp
	g++ pseudo_echallan.cpp Functions/functions.cpp -o pseudo_echallan `pkg-config --cflags --libs opencv4`

run-pseudo: pseudo_echallan
	./pseudo_echallan


registration: Registrations/registration.cpp
	g++ Registrations/registration.cpp Functions/functions.cpp -o Registrations/registration `pkg-config --cflags --libs opencv4`

run-registration: registration
	./Registrations/registration


bank: Finances/bank.cpp
	g++ Finances/bank.cpp Functions/functions.cpp -o Finances/bank `pkg-config --cflags --libs opencv4`

run-bank: bank
	./Finances/bank


payChallan: Finances/payChallan.cpp
	g++ Finances/payChallan.cpp Functions/functions.cpp -o Finances/payChallan `pkg-config --cflags --libs opencv4`

run-pay: payChallan
	./Finances/payChallan
	

all: cv admin registration bank challan

clean:
	rm -f cv admin registration bank challan
