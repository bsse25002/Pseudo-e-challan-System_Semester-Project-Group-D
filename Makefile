challan: Challans/challan.cpp
	g++ Challans/challan.cpp Functions/functions.cpp -o challan `pkg-config --cflags --libs opencv4`

run-challan: challan
	./challan


pseudo_echallan: pseudo_echallan.cpp
	g++ pseudo_echallan.cpp Functions/functions.cpp -o pseudo_echallan `pkg-config --cflags --libs opencv4`

run-pseudo: pseudo_echallan
	./pseudo_echallan


registration: Registrations/registration.cpp
	g++ Registrations/registration.cpp Functions/functions.cpp -o registration `pkg-config --cflags --libs opencv4`

run-registration: registration
	./registration


bank: Finances/bank.cpp
	g++ Finances/bank.cpp Functions/functions.cpp -o bank `pkg-config --cflags --libs opencv4`

run-bank: bank
	./bank


payChallan: Finances/payChallan.cpp
	g++ Finances/payChallan.cpp Functions/functions.cpp -o payChallan `pkg-config --cflags --libs opencv4`

run-pay: payChallan
	./payChallan
	

all: cv registration bank challan

clean:
	rm -f cv registration bank challan
