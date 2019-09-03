#ifndef AC_H_
#define AC_H_
#include <systemc.h>
#include <iostream>
using namespace std;

template <unsigned int Nbits>
SC_MODULE(acBlock){

	sc_in<bool>				ac_load;
	sc_in<sc_uint<Nbits>>	ula_out;
	sc_out<sc_uint<Nbits>>	ac_out;

	sc_uint<Nbits>			_buffer;

	void ac(){

		if (ac_load.read() == 1)
		{
			_buffer = ula_out.read();
			cout << "@" << sc_time_stamp() << " _buffer = "<<_buffer << endl;
		}
		ac_out.write(_buffer);

		cout << "@" << sc_time_stamp() << " ac_out = "<<ac_out << endl;
	}

	SC_CTOR(acBlock){
		SC_METHOD(ac);
		sensitive << ac_load;
	}

	static int testbench(const char * testbench_file){
		//Sinais

		sc_signal<bool>				_ac_load;
		sc_signal<sc_uint<Nbits>>	_ula_out;
		sc_signal<sc_uint<Nbits>>	_ac_out;

		//Interligando os Sinais
		acBlock<Nbits> ac("ac");

		ac.ac_load(_ac_load);
		ac.ula_out(_ula_out);
		ac.ac_out(_ac_out);

		sc_start(1, SC_NS);	//Roda 1ns de Simula��o

		// Open VCD file
		sc_trace_file *wf = sc_create_vcd_trace_file(testbench_file);

		// Dump the desired signals
		sc_trace(wf, _ac_load, "ac_load");
		sc_trace(wf, _ula_out, "ula_out");
		sc_trace(wf, _ac_out, "ac_out");

		// Initialize all variables
		_ac_load = 0;
		_ula_out = 0;
		sc_start(1, SC_NS);

		//EXECUTE SIMULATION *********************************
		_ac_load = 1;
		sc_start(1, SC_NS);
		_ac_load = 0;

		_ula_out = 5;
		sc_start(1, SC_NS);
		_ac_load = 1;

		sc_start(1, SC_NS);
		_ac_load = 0;

		_ula_out = 2;
		sc_start(1, SC_NS);
		// ***************************************************
		//Terminate simulation
		cout << "@" << sc_time_stamp() <<" Terminating simulation" << endl;
		sc_close_vcd_trace_file(wf);
		return 0;
	}
};

#endif /* AC_H_ */
