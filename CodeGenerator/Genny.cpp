#ifndef __GENERATOR_H
#define __GENERATOR_H


#include <set>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

typedef struct _varDescriptor {
	std::string var;
	std::set<std::string> isIn;	
	int weight;
	int offset = -1;

} varDescriptor;

typedef struct _regDescriptor {
	bool usable = true;
	std::set<std::string> has;	
	int score;

} regDescriptor;




class Descriptor {
	public:
		std::vector <varDescriptor*> vars;
		regDescriptor *R[32];

		Descriptor() {
			
			this->R[0]->usable = false;
			// Aqui falta colocar los que no se pueden
			// usar
		}

		regDescriptor* getRegDescr(int i) {
			return this->R[i];
		}

		
		set<string> getRDset(int i) {
			this->R[i]->has;
		}


		varDescriptor* getVarDescr(string var) {

			vector <varDescriptor*> v = this->vars;
			for(vector<varDescriptor*>::iterator it = v.begin(); it != v.end(); ++it) {
				if ( (*it)->var == var ) {
					return *it;
				}	
			}
			cout << "Error en GET_VAR_DESCRIPTOR";
		}

		void addVarDescr(string var) {
		
			varDescriptor *nVD = new varDescriptor;
			nVD->var = var;
			nVD->isIn.insert(var);
			nVD->weight = 1;
			this->vars.push_back(nVD);

		}

		void addtmpVarDescr(string var) {

			varDescriptor *nVD = new varDescriptor;
			nVD->weight = 1;
			nVD->var = var;
			nVD->weight = 10;
			this->vars.push_back(nVD);
		}



		void addVarInVar(string var) {
			getVarDescr(var)->isIn.insert(var);
		}

		void VarSetOffset(string var, int offset) {
			getVarDescr(var)->offset = offset;
		}


		int getVarWeight(string var) {
			return getVarDescr(var)->weight;
		}

		int getRegScore(int i) {
			return getRegDescr(i)->score;
		}

		void setRDscore(int i, int score) {
			getRegDescr(i)->score = score;
		}

		void addRDscore(int i, int score) {
			getRegDescr(i)->score += score;
		}

		int getLowestRegister() {

			int min = 2000000;
			int r = 0;
			for (int i = 0; i < 32; ++i) {

				if (getRegScore(i) < min) {
					r = i;
					min = getRegScore(i);
				}
			}
			return r;
		}



		void addVarInR(string var, int ri, int only) {
			varDescriptor* vd = getVarDescr(var);
  			
  			// Borro todo para que la variable que entre sea la unica
  			if (only == 1) {
  				vd->isIn.clear();
  			}

  			vd->isIn.insert( to_string(ri) );
		}

		bool isTemp(string var) {
			return var[0] == '_';
		}

		vector<int> emptyRegisters() {
			
			vector<int> freeRegs;
			for (int i = 0; i < 32; ++i) {
				if (! getRegDescr(i)->has.empty() ) {
					freeRegs.push_back(i);
				}
			}

		}

		// Un registro cambia de valor, eliminar su referencia en las variables
		void registerChanged(string var, int ri) {
			varDescriptor* vd = getVarDescr(var);
			vd->isIn.erase( to_string(ri) );
		}



		// LD Ri, var
		void load(int i, string var) {
			
			regDescriptor* R = getRegDescr(i);

			// Actualizo los Resgistros
			// Aunque no se si esta parte pasa aqui
			while (!R->has.empty()) {
   		
    			registerChanged(*( R->has.begin() ), i);
    			R->has.erase(R->has.begin());
  			}
  			R->has.insert( var );

  			// Actualizo las variables
  			addVarInR(var, i, 0);
  			setRDscore(i, getVarWeight(var));

		}	
		
		// LD Ri, y  ( x := y )
		void copy(int i, string varY, string varX) {

			//load(i, varY); NO siempre se necesita
			
			getRegDescr(i)->has.insert(varX);
			addVarInR(varX, i, 1);
			addRDscore(i, getVarWeight(varX));
		}

		
		// ST R, x
		void store(string var, int i) {
			addVarInR(var, i, 0);
			addVarInVar(var);
		}

		// OPP Ri, Rj, Rk
		// En realidad solo importa el R destion y la variable destino
		void operation(int i, string var) {

			regDescriptor* R = getRegDescr(i);
			// Actualizo los Resgistros
			while (!R->has.empty()) {
   		
    			registerChanged(*( R->has.begin() ), i);
    			R->has.erase(R->has.begin());
  			}
  			R->has.insert( var );

  			// variable recien calculada solo puede estar en ese registro
  			addVarInR(var, i, 1);

		}


		bool isInSet(string var, set<string> s) {
			set<string>::iterator it;
			it = s.find(var);
			return it != s.end();
		}


		bool isVarInReg(string var, int i) {
			return isInSet(var, getRDset(i));

		}



		// Me dice si la variable esta en otra ubicación para reciclarla
		bool IsVarAvailable (int r, string var) {
			
			regDescriptor* R = getRegDescr(r);

			//Busqueda en los registros
			for (int i = 0; i < 32; ++i) {
				if ( isInSet(var, R->has) && r != i) {
					return true;
				}
			}

			//Busqueda en las variables
			vector<varDescriptor*> v = this->vars;
			for (vector<varDescriptor*>::iterator it = v.begin() ; it != v.end(); ++it) {
				if ( isInSet(var, (*it)->isIn) ) {
					return true;
				}
			}
			return false;
		}



		// Creo que se puede implementar con get reg para operandos y get reg para destinos
		int getRegOperand (string op1, string op2, string destino) {


			// Ya esta en un registro?
			for (int i = 0; i < 32; ++i) {
				if ( isInSet(op1, getRegDescr(i)->has ) ) {
					return i;
					// No Genero nada
				}
			}

			// Hay registros libres?
			vector<int> freeRegs = emptyRegisters();
			if ( freeRegs.size() > 0 ) {
				return freeRegs.front();
				// Genero LD Ri y
			}
			
			// Puedo reciclar? ---------
			
			// Si esta disponible en otro sitio
			for (int i = 0; i < 32; ++i) {
				if ( IsVarAvailable(i, *(getRegDescr(i)->has.begin()) ) && getRegDescr(i)->has.size() == 1 ) {
					return i;
				}
			}

			// ----------------------------------------------------------- Debo chequear que este sola? --------
			// Si voy a cambiar el valor a algo que esta en un registro
			// -> variable destino esta en algun resgistro sola? la voy a cambiar
			//Busqueda en los registros
			for (int i = 0; i < 32; ++i) {
				if ( isInSet(destino, getRegDescr(i)->has) && (destino != op1 && destino != op2 ) ) {
					return i;
				}
			}


			// Spill ---------------
			// consigo el candidato
			int lowest = getLowestRegister();

			set<string> s = getRegDescr(lowest)->has;
			for (set<string>::iterator it = s.begin() ; it != s.end(); ++it) {
				// si es temporal
				if ( isTemp( *(it) ) ) {
					// ST R offset($fp)
				} else {
					// Genera ST x, R
				}
			}
			return lowest;
		}

		int getRegDestino (string destino, string op1, string op2) {

			
			// Si un registro solo contiene el resultado que voy a cambiar
			for (int i = 0; i < 33; ++i) {
				if (	isInSet(destino, getRegDescr(i)->has) 
						&& getRegDescr(i)->has.size() == 1 ) {
					return i;
				}
			}

			// Usar uno de los registros de los operandos para el resultado
			for (int i = 0; i < 33; ++i) {
				if ( (	isInSet(op1, getRegDescr(i)->has) 
						|| isInSet(op2, getRegDescr(i)->has) 
					 )
						&& getRegDescr(i)->has.size() == 1 ) {
					return i;
				}
			}

			// Hay registros libres?
			vector<int> freeRegs = this->emptyRegisters();
			if ( freeRegs.size() > 0 ) {
				 return freeRegs.front();
			}
			
			// Puedo reciclar?
			for (int i = 0; i < 33; ++i) {
				if ( IsVarAvailable(i, *(getRegDescr(i)->has.begin()) ) && getRegDescr(i)->has.size() == 1 ) {
					return i;
				}
			}

			// Spill ---------------
			// consigo el candidato
			int lowest = getLowestRegister();

			set<string> s = getRegDescr(lowest)->has;
			for (set<string>::iterator it = s.begin() ; it != s.end(); ++it) {
				// si es temporal
				if ( isTemp( *(it) ) ) {
					// ST R offset($fp)
				} else {
					// Genera ST x, R
				}
			}

			return lowest;

		}

		typedef enum {ADD, SUB, MUL} Operation;
		

		void pr(int i) {
			printf("$R%d ", i);
		}

		void print_op(Operation o) {

			switch (o) {
				case ADD:
					printf("add ");
				break;
				
				case SUB:
					printf("add ");
				break;

				case MUL:
					printf("mul ");
				break;

				default:
					printf("no hay caso %d\n", o);
				break;
			}

		}


		void gen_ld(char type, int ri,  const char *s) {
			printf("l%c $R%d %s\n", type, ri, s);
		}

		void gen_ldoff(char type, int ri, int offset, int rj) {
			printf("l%c $R%d %d($R%d)\n", type, ri, offset, rj);
		}


		// OPP RT Ri Rj
		void gen_bin_op(Operation op, int rt, int ri, int rj) {
		
			print_op(op); pr(rt); pr(ri); pr(rj);

		}



		// // x := a[i]
		// void GenArrayR(string varX, string i) {

		// 	int ri = getRegOperand(i, varX, "");

		// 	if ( !isInSet( i, getRDset(ri)) ) {
		// 		gen_ld('d', ri, i.c_str());
		// 	}

		// 	int rx = getRegDestino(varX, i, "");
		// 	if ( !isInSet( i, getRDset(rx)) ) {
		// 		gen_ld('d', rx, varX.c_str());
		// 	}

		// 	gen_ldoff('d', ri, offset, rj) {

		// 	}


		// }

		// x = y + z
		void Gen_int_sum(string target, string op1, string op2) {

			int ri = getRegOperand(op1, op2, target);

			if ( !isVarInReg(op1, ri) ) {
				gen_ld('d', ri, op1.c_str());
			}			

			int rj = getRegOperand(op2, op1, target);

			if ( !isVarInReg(op1, rj) ) {
				gen_ld('d', ri, op2.c_str());
			}

			int rd = getRegDestino(target, op1, op2);

			gen_bin_op(ADD, rd, ri, rj);

		}

		void Gen_int_mult(string target, string op1, string op2) {

			int ri = getRegOperand(op1, op2, target);

			if ( !isVarInReg(op1, ri) ) {
				gen_ld('d', ri, op1.c_str());
			}			

			int rj = getRegOperand(op2, op1, target);

			if ( !isVarInReg(op1, rj) ) {
				gen_ld('d', ri, op2.c_str());
			}

			int rd = getRegDestino(target, op1, op2);

			gen_bin_op(MUL, rd, ri, rj);

		}





		void Gen_int_sub(string target, string op1, string op2) {

			int ri = getRegOperand(op1, op2, target);

			if ( !isVarInReg(op1, ri) ) {
				gen_ld('d', ri, op1.c_str());
			}			

			int rj = getRegOperand(op2, op1, target);

			if ( !isVarInReg(op1, rj) ) {
				gen_ld('d', ri, op2.c_str());
			}

			int rd = getRegDestino(target, op1, op2);

			gen_bin_op(SUB, rd, ri, rj);

		}

		void Gen_prologF(int locals) {

			printf("add $sp $sp -12\n");	
			printf("sw $ra 4($sp)\n");
			printf("sw $fp 0($sp)\n"); 
        	printf("move $fp $sp \n");
        	printf("add $sp $sp -%d", locals);

        }

        void Gen_epilogF(int locals) {

        	printf("add $sp $sp %d ", locals);
        	
        	printf("lw $fp 0($sp)\n");
			printf("add $sp $sp 4\n");
			
			printf("lw $ra 0($sp)\n");
			printf("add $sp $sp 4\n");

			printf("jr $ra\n");

        }


        void Gen_return(char* ret) {

        	int r = getRegDestino(ret, "", "");
        	if ( !isVarInReg(ret, r) ) {
        		printf("ld $R%d %s\n", r, ret);
        	}
        	
        	printf("lw 8($fp) $R%d", r);
        }

        void Gen_goto(char* label) {

        	printf("b %s\n", label);

        }


        void Gen_label(char* label) {

        	printf("%s:\n", label);
        }


        void condition()


















}; // Class


int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}









#endif