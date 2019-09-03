#ifndef UC_H_
#define UC_H_
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

typedef enum states {
	FECHT,
	DECODE,
	STA_LOAD_ADDR,
	STA_INSTRUCTION,
	LDA_LOAD_ADDR,
	LDA_LOAD_VALUE,
	LDA_INSTRUCTION,
	LDA_AC_LOAD,
	ADD_LOAD_ADDR,
	ADD_LOAD_VALUE,
	ADD_INSTRUCTION,
	ADD_AC_LOAD,
	OR_LOAD_ADDR,
	OR_LOAD_VALUE,
	OR_INSTRUCTION,
	OR_AC_LOAD,
	AND_LOAD_ADDR,
	AND_LOAD_VALUE,
	AND_INSTRUCTION,
	AND_AC_LOAD,
	NOT_INSTRUCTION,
	NOT_AC_LOAD,
	JMP_LOAD_ADDR,
	JMP_LOAD_VALUE,
	JN_LOAD_ADDR,
	JN_LOAD_VALUE,
	JZ_LOAD_ADDR,
	JZ_LOAD_VALUE,
	HLT_,
	END_EXECUTION
}state_t;


template <unsigned int Nbits>
SC_MODULE(ucBlock){
	sc_in_clk				clock;
	sc_in<bool>				ula_flagZ;
	sc_in<bool>				ula_flagN;
	sc_in<sc_uint<Nbits>>	mem_out;

	sc_out<sc_uint<Nbits>>	ula_op;
	sc_out<bool>			ac_load;
	sc_out<bool>			pc_inc;
	sc_out<bool>			pc_load;
	sc_out<bool>			mux_sel;
	sc_out<bool>			mem_wr;
	sc_out<bool>			mem_rd;

	state_t	 _next_state = FECHT;
	state_t	 _current_state = FECHT;

	void uc_stateDo(){ // sensivel a _state
		switch (_state)
				{
				 case FECHT:
					 pc_inc.write(0);
					 pc_load.write(0);
					 mem_wr.write(1);
					 mem_rd.write(0);
					 mux_sel.write(0);
					 ac_load.write(0);
					 ula_op.write(NOP);
					 break;
				 case DECODE:
					 pc_inc.write(1);
					 break;
				 case STA_LOAD_ADDR:
					 pc_inc.write(0);
					 break;
				 case STA_INSTRUCTION:
					 mem_wr.write(1);
					 mem_rd.write(0);
					 mux_sel.write(1);
					 break;
				 case LDA_LOAD_ADDR:
					 pc_inc.write(0);
					 break;
				 case LDA_LOAD_VALUE:
					 mux_sel.write(1);
					 break;
				 case LDA_INSTRUCTION:
					 mem_rd.write(0);
					 mux_sel.write(0);
					 ula_op.write(LDA);
					 break;
				 case LDA_AC_LOAD:
					 ac_load.write(1);
					 break;
				 case ADD_LOAD_ADDR:
					 pc_inc.write(0);
					 break;
				 case ADD_LOAD_VALUE:
					 mux_sel.write(1);
					 break;
				 case ADD_INSTRUCTION:
					 mem_rd.write(0);
					 mux_sel.write(0);
					 ula_op.write(ADD);
					 break;
				 case ADD_AC_LOAD:
					 ac_load.write(1);
					 break;
				 case OR_LOAD_ADDR:
					 pc_inc.write(0);
					 break;
				 case OR_LOAD_VALUE:
					 mux_sel.write(1);
					 break;
				 case OR_INSTRUCTION:
					 mem_rd.write(0);
					 mux_sel.write(0);
					 ula_op.write(OR);
					 break;
				 case OR_AC_LOAD:
					 ac_load.write(1);
					 break;
				 case AND_LOAD_ADDR:
					 pc_inc.write(0);
					 break;
				 case AND_LOAD_VALUE:
					 mux_sel.write(1);
					 break;
				 case AND_INSTRUCTION:
					 mem_rd.write(0);
					 mux_sel.write(0);
					 ula_op.write(AND);
					 break;
				 case AND_AC_LOAD:
					 ac_load.write(1);
					 break;
				 case NOT_INSTRUCTION:
					 mem_rd.write(0);
					 mux_sel.write(0);
					 ula_op.write(NOT);
					 break;
				 case NOT_AC_LOAD:
					 ac_load.write(1);
					 ula_op.write(0);
					 break;
				 case JMP_LOAD_ADDR:
					 pc_inc.write(0);
					 break;
				 case JMP_LOAD_VALUE:
					 ac_load.write(1);
					 break;
				 case JN_LOAD_ADDR:
					 pc_inc.write(0);
					 break;
				 case JN_LOAD_VALUE:
					 ac_load.write(1);
					 break;
				 case JZ_LOAD_ADDR:
					 pc_inc.write(0);
					 break;
				 case JZ_LOAD_VALUE:
					 ac_load.write(1);
					 break;
				 case HLT_:
					 pc_inc.write(0);
					 break;
				 case END_EXECUTION:
					 pc_inc.write(1);
					 pc_load.write(0);
					 mem_wr.write(0);
					 mem_rd.write(0);
					 mux_sel.write(0);
					 ac_load.write(0);
					 ula_op.write(NOP);
					 break;
				 default:

				}


	}

	void uc_stateChange(){ // sensivel ao clock
		switch (_current_state)
		{
		 case FECHT:
			 _next_state = DECODE;
			 break;
		 case DECODE:
			 if (mem_out.read() == NOP){
				 _next_state = FECHT;
			 }
			 else if (mem_out.read() == STA){
				 _next_state = STA_LOAD_ADDR;
			 }
			 else if (mem_out.read() == LDA){
				 _next_state = LDA_LOAD_ADDR;
			 }
			 else if (mem_out.read() == ADD){
				 _next_state = ADD_LOAD_ADDR;
			 }
			 else if (mem_out.read() == OR){
				 _next_state = OR_LOAD_ADDR;
			 }
			 else if (mem_out.read() == AND){
				 _next_state = AND_LOAD_ADDR;
			 }
			 else if (mem_out.read() == NOT){
				 _next_state = NOT_INSTRUCTION;
			 }
			 else if (mem_out.read() == JMP){
				 _next_state = JMP_LOAD_ADDR;
			 }
			 else if (mem_out.read() == JZ){
				 _next_state = JZ_LOAD_ADDR;
			 }
			 else if (mem_out.read() == JN){
				 _next_state = JN_LOAD_ADDR;
			 }
			 else if (mem_out.read() == HLT){
				 _next_state = HLT_;
			 }
			 else
				 _next_state = FECHT;

			 break;
		 case STA_LOAD_ADDR:
			 break;
		 case STA_INSTRUCTION:
			 break;
		 case LDA_LOAD_ADDR:
			 break;
		 case LDA_LOAD_VALUE:
			 break;
		 case LDA_INSTRUCTION:
			 break;
		 case LDA_AC_LOAD:
			 break;
		 case ADD_LOAD_ADDR:
			 break;
		 case ADD_LOAD_VALUE:
			 break;
		 case ADD_INSTRUCTION:
			 break;
		 case ADD_AC_LOAD:
			 break;
		 case OR_LOAD_ADDR:
			 break;
		 case OR_LOAD_VALUE:
			 break;
		 case OR_INSTRUCTION:
			 break;
		 case OR_AC_LOAD:
			 break;
		 case AND_LOAD_ADDR:
			 break;
		 case AND_LOAD_VALUE:
			 break;
		 case AND_INSTRUCTION:
			 break;
		 case AND_AC_LOAD:
			 break;
		 case NOT_INSTRUCTION:
			 break;
		 case NOT_AC_LOAD:
			 break;
		 case JMP_LOAD_ADDR:
			 break;
		 case JMP_LOAD_VALUE:
			 break;
		 case JN_LOAD_ADDR:
			 break;
		 case JN_LOAD_VALUE:
			 break;
		 case JZ_LOAD_ADDR:
			 break;
		 case JZ_LOAD_VALUE:
			 break;
		 case HLT:
			 break;
		 case END_EXECUTION:
			 break;
		 default:

		}
		_current_state = _next_state;
	}

	SC_CTOR(ucBlock){
		SC_METHOD(uc);
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

#endif /* UC_H_ */
