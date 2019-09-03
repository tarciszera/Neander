#ifndef MUX_H_
#define MUX_H_
#include <systemc.h>
#include <iostream>
using namespace std;

template <unsigned int Nbits>
SC_MODULE(muxBlock){

	sc_in<bool>				mux_sel;
	sc_in<sc_uint<Nbits>>	pc_out;
	sc_in<sc_uint<Nbits>>	mem_out;
	sc_out<sc_uint<Nbits>>	mux_out;

	sc_uint<Nbits>			_buffer;

	void mux(){
		if (mux_sel.read() == 0)
		{
			_buffer = pc_out.read();
			cout << "@" << sc_time_stamp() << " data = pc_out" << endl;
		}
		else
		{
			_buffer = mem_out.read();
			cout << "@" << sc_time_stamp() << " data = mem_out" << endl;
		}
		mux_out.write(_buffer);
	}

	SC_CTOR(muxBlock){
		SC_METHOD(mux);
		sensitive << pc_out << mem_out;
		sensitive << mux_sel;
	}

	static int testbench(const char * testbench_file){
		//Sinais
		sc_signal<bool> 	_mux_sel;
		sc_signal<sc_uint<Nbits>> _pc_out;
		sc_signal<sc_uint<Nbits>> _mem_out;
		sc_signal<sc_uint<Nbits>> _mux_out;

		//Interligando os Sinais
		muxBlock<Nbits> mux("mux");

		mux.mux_sel(_mux_sel);
		mux.pc_out(_pc_out);
		mux.mem_out(_mem_out);
		mux.mux_out(_mux_out);

		sc_start(1, SC_NS);	//Roda 1ns de Simula��o

		// Open VCD file
		sc_trace_file *wf = sc_create_vcd_trace_file(testbench_file);

		// Dump the desired signals
		sc_trace(wf, _mux_sel, "mux_sel");
		sc_trace(wf, _pc_out, "pc_out");
		sc_trace(wf, _mem_out, "mem_out");
		sc_trace(wf, _mux_out, "mux_out");

		// Initialize all variables
		_pc_out = 1;
		_mem_out = 0;
		sc_start(1, SC_NS);

		//EXECUTE SIMULATION *********************************
		_mux_sel = 1;
		sc_start(1, SC_NS);
		
		_mux_sel = 0;
		sc_start(1, SC_NS);
		
		// ***************************************************
		//Terminate simulation
		cout << "@" << sc_time_stamp() <<" Terminating simulation" << endl;
		sc_close_vcd_trace_file(wf);
		return 0;
	}
};

#endif /* MUX_H_ */
