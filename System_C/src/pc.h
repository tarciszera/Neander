#ifndef PC_H_
#define PC_H_
#include <systemc.h>
#include <iostream>
using namespace std;

template <unsigned int Nbits>
SC_MODULE(pcBlock){
	sc_in_clk				clock;
	sc_in<bool>				pc_inc;
	sc_in<bool>				pc_load;
	sc_in<sc_uint<Nbits>>	mem_out;
	sc_out<sc_uint<Nbits>>	pc_out;

	sc_uint<Nbits>			_buffer;

	void pc(){

		if (pc_inc.read() == 1){
			_buffer = pc_out.read() + 1;
		}
		else if (pc_load.read() == 1){
			_buffer = mem_out.read();
		}
		pc_out.write(_buffer);
		cout << "@" << sc_time_stamp() << " pc_out = "<<pc_out << endl;
	}

	SC_CTOR(pcBlock){
		SC_METHOD(pc);
		sensitive << clock.pos();
	}

	static int testbench(const char * testbench_file){
		//Sinais

		sc_clock					_clock("clk", 2, 0.5, 1, false);
		sc_signal<bool>				_pc_inc;
		sc_signal<bool>				_pc_load;
		sc_signal<sc_uint<Nbits>>	_mem_out;
		sc_signal<sc_uint<Nbits>>	_pc_out;

		//Interligando os Sinais
		pcBlock<Nbits> pc("pc");

		pc.clock(_clock);
		pc.pc_load(_pc_load);
		pc.pc_inc(_pc_inc);
		pc.mem_out(_mem_out);
		pc.pc_out(_pc_out);

		sc_start(1, SC_NS);	//Roda 1ns de Simula��o

		// Open VCD file
		sc_trace_file *wf = sc_create_vcd_trace_file(testbench_file);

		// Dump the desired signals
		sc_trace(wf, _clock, "_clock");
		sc_trace(wf, _pc_inc, "_pc_inc");
		sc_trace(wf, _pc_load, "_pc_inc");
		sc_trace(wf, _mem_out, "_mem_out");
		sc_trace(wf, _pc_out, "_pc_out");

		// Initialize all variables
		_pc_inc = 0;
		_pc_load = 1;
		_mem_out = 3;
		sc_start(2, SC_NS);

		_mem_out = 5;
		//EXECUTE SIMULATION *********************************
		_pc_load = 0;
		sc_start(2, SC_NS);
		_pc_inc = 1;
		sc_start(10, SC_NS);
		// ***************************************************
		//Terminate simulation
		cout << "@" << sc_time_stamp() <<" Terminating simulation" << endl;
		sc_close_vcd_trace_file(wf);
		return 0;
	}
};

#endif /* PC_H_ */
