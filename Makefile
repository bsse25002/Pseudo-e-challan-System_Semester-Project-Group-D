challan: challan.cpp
	g++ challan.cpp -o challan `pkg-config --cflags --libs opencv4`

run-challan: challan
	./challan


pseudo_echallan: pseudo_echallan.cpp
	g++ pseudo_echallan.cpp -o pseudo_echallan

run-pseudo: pseudo_echallan
	./pseudo_echallan


registration: registration.cpp
	g++ registration.cpp -o registration

run-registration: registration
	./registration


bank: bank.cpp
	g++ bank.cpp -o bank

run-bank: bank
	./bank


payChallan: payChallan.cpp
	g++ bank.cpp -o bank

run-pay: payChallan
	./payChallan
	

all: cv registration bank challan

clean:
	rm -f cv registration bank challan
