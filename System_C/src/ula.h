#ifndef ULA_H_
#define ULA_H_
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
SC_MODULE(ulaBlock){

	sc_in<sc_uint<Nbits>>	mem_out;
	sc_in<sc_uint<Nbits>>	ac_out;
	sc_in<sc_uint<Nbits>>	ula_op;
	sc_out<sc_int<Nbits>>	ula_out;
	sc_out<bool>			ula_flagZ;
	sc_out<bool>			ula_flagN;

	sc_int<Nbits>			_buffer;

	void ula(){

		if (ula_op.read() == LDA){
			_buffer = mem_out.read();
		}
		else if (ula_op.read() == ADD){
			_buffer = mem_out.read() + ac_out.read();
		}
		else if (ula_op.read() == OR){
			_buffer = mem_out.read() | ac_out.read();
		}
		else if (ula_op.read() == AND){
			_buffer = mem_out.read() & ac_out.read();
		}
		else if (ula_op.read() == NOT){
			_buffer = ~ac_out.read();
		}

		ula_out.write(_buffer);

		if (_buffer < 0){
			ula_flagZ.write(0);
			ula_flagN.write(1);
		}
		else if (_buffer == 0){
			ula_flagZ.write(1);
			ula_flagN.write(0);
		}
		else{
			ula_flagZ.write(0);
			ula_flagN.write(0);
		}

		cout << "@" << sc_time_stamp() << " ula_out = "<<ula_out << endl;
		cout << "@" << sc_time_stamp() << " flagZ = "<<ula_flagZ << endl;
		cout << "@" << sc_time_stamp() << " flagN = "<<ula_flagN << endl;

	}

	SC_CTOR(ulaBlock){
		SC_METHOD(ula);
		sensitive << ula_op << ac_out << mem_out;
	}

	static int testbench(const char * testbench_file){
		//Sinais

		sc_signal<sc_uint<Nbits>>	_mem_out;
		sc_signal<sc_uint<Nbits>>	_ac_out;
		sc_signal<sc_uint<Nbits>>	_ula_op;
		sc_signal<sc_int<Nbits>>	_ula_out;
		sc_signal<bool>			_ula_flagZ;
		sc_signal<bool>			_ula_flagN;

		//Interligando os Sinais
		ulaBlock<Nbits> ula("ula");

		ula.mem_out(_mem_out);
		ula.ac_out(_ac_out);
		ula.ula_op(_ula_op);
		ula.ula_out(_ula_out);
		ula.ula_flagZ(_ula_flagZ);
		ula.ula_flagN(_ula_flagN);

		sc_start(1, SC_NS);	//Roda 1ns de Simula��o

		// Open VCD file
		sc_trace_file *wf = sc_create_vcd_trace_file(testbench_file);

		// Dump the desired signals
		sc_trace(wf, _mem_out, "_mem_out");
		sc_trace(wf, _ac_out, "_ac_out");
		sc_trace(wf, _ula_op, "_ula_op");
		sc_trace(wf, _ula_out, "_ula_out");
		sc_trace(wf, _ula_flagN, "_ula_flagN");
		sc_trace(wf, _ula_flagZ, "_ula_flagZ");

		// Initialize all variables
		_ac_out = 5;
		_mem_out = 3;
		_ula_op = LDA;
		sc_start(2, SC_NS);

		//EXECUTE SIMULATION *********************************

		_ula_op = ADD;
		sc_start(2, SC_NS);
		_ula_op = OR;
		sc_start(2, SC_NS);
		_ula_op = AND;
		sc_start(2, SC_NS);
		_ula_op = NOT;
		sc_start(2, SC_NS);

		_ula_op = NOP;
		sc_start(2, SC_NS);
		// ***************************************************
		//Terminate simulation
		cout << "@" << sc_time_stamp() <<" Terminating simulation" << endl;
		sc_close_vcd_trace_file(wf);
		return 0;
	}
};

#endif /* PC_H_ */
