#ifndef MEM_H_
#define MEM_H_
#include <systemc.h>
#include <iostream>
using namespace std;

#ifndef NOP

#define NOP		0x00
#define STA		0x10
#define LDA		0x20
#define ADD		0x30
#define OR		0x40
#define AND		0x50
#define NOT		0x60
#define JMP		0x80
#define JN		0x90
#define JZ		0xA0
#define HLT		0xF0

#endif

template <unsigned int Nbits>
SC_MODULE(memBlock){

	sc_in_clk				clock;
	sc_in<sc_uint<Nbits>>	mux_out;
	sc_in<sc_uint<Nbits>>	ac_out;
	sc_in<bool>				mem_rd;
	sc_in<bool>				mem_wr;
	sc_out<sc_uint<Nbits>>	mem_out;

	sc_uint<Nbits>			_mem[256];

	void mem(){

		if (mem_rd.read() == 1){
			mem_out.write(_mem[mux_out.read()]);
		}

		if (mem_wr.read() == 1){
			_mem[mux_out.read()] = ac_out.read();
		}

		cout << "@" << sc_time_stamp() << " mem_out = "<<mem_out << endl;
		cout << "@" << sc_time_stamp() << " mem = "<<_mem[mux_out.read()]  << endl;

	}

	SC_CTOR(memBlock){
		SC_METHOD(mem);
		sensitive << clock.pos();

		_mem[0]=STA;
		_mem[1]=0;
		_mem[2]=LDA;
		_mem[3]=2;
		_mem[4]=ADD;
		_mem[5]=4;
		_mem[6]=OR;
		_mem[7]=255;
		_mem[8]=AND;
		_mem[9]=254;
		_mem[10]=NOT;
		_mem[11]=JMP;
		_mem[12]=14;
		_mem[13]=HLT;
		_mem[14]=JN;
		_mem[15]=17;
		_mem[16]=HLT;
		_mem[17]=LDA;
		_mem[18]=55;
		_mem[19]=JZ;
		_mem[20]=16;
		_mem[255]=1;
		_mem[254]=126;

	}
	static int testbench(const char * testbench_file){
		//Sinais

		sc_clock					_clock("clk", 2, 0.5, 1, false);
		sc_signal<sc_uint<Nbits>>	_mux_out;
		sc_signal<sc_uint<Nbits>>	_ac_out;
		sc_signal<bool>				_mem_rd;
		sc_signal<bool>				_mem_wr;
		sc_signal<sc_uint<Nbits>>	_mem_out;

		//Interligando os Sinais
		memBlock<Nbits> mem("ula");

		mem.clock(_clock);
		mem.mux_out(_mux_out);
		mem.ac_out(_ac_out);
		mem.mem_rd(_mem_rd);
		mem.mem_wr(_mem_wr);
		mem.mem_out(_mem_out);

		sc_start(1, SC_NS);	//Roda 1ns de Simula��o

		// Open VCD file
		sc_trace_file *wf = sc_create_vcd_trace_file(testbench_file);

		// Dump the desired signals
		sc_trace(wf, _clock, "_clock");
		sc_trace(wf, _mux_out, "_mux_out");
		sc_trace(wf, _ac_out, "_ac_out");
		sc_trace(wf, _mem_rd, "_mem_rd");
		sc_trace(wf, _mem_wr, "_mem_wr");
		sc_trace(wf, _mem_out, "_mem_out");

		// Initialize all variables
		_mux_out = 5;
		_ac_out = 3;
		_mem_rd = 0;
		_mem_wr = 0;

		sc_start(2, SC_NS);

		//EXECUTE SIMULATION *********************************

		_mem_rd = 1;
		sc_start(2, SC_NS);
		_mem_rd = 0;
		_mem_wr = 1;
		sc_start(2, SC_NS);
		_mem_wr = 0;
		sc_start(2, SC_NS);
		// ***************************************************
		//Terminate simulation
		cout << "@" << sc_time_stamp() <<" Terminating simulation" << endl;
		sc_close_vcd_trace_file(wf);
		return 0;
	}
};

#endif /* MEM_H_ */
