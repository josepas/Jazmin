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
} varDescriptor;



class Descriptor {
	public:
		std::vector <varDescriptor*> vars;
		std::set<std::string> R[33];

		Descriptor() {
			this->R[0].insert("__");
			//this->R[0].insert("__");
			//this->R[0].insert("__");
			//this->R[0].insert("__");

			// Aqui falta colocar los que no se pueden
			// usar
		}

		void addVarDescr(string var) {
		
			varDescriptor *nVD = new varDescriptor;
			nVD->var = var;
			nVD->isIn.insert(var);
			this->vars.push_back(nVD);
		}

		void addtmpVarDescr(string var) {

			varDescriptor *nVD = new varDescriptor;
			nVD->var = var;
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

		void addAvailableAtR(string var, int i, int only) {
			varDescriptor* vd = getVarDescr(var);
  			
  			//  ------------------ Puedo hacer este clear tranquilamente? -----------------------------
  			//  ------------------ creo que si
  			if (only == 1) {
  				vd->isIn.clear();
  			}

  			vd->isIn.insert( to_string(i) );
		}

		vector<int> emptyRegisters() {
			
			vector<int> freeRegs;
			for (int i = 0; i < 33; ++i) {
				if (! this->R[i].empty() ) {
					freeRegs.push_back(i);
				}
			}

		}

		// Un registro cambia de valor, eliminar sus referencias en las variables
		void registerChanged(string var, int i) {
			varDescriptor* vd = getVarDescr(var);
			vd->isIn.erase( to_string(i) );
		}



		// LD Ri, var
		void load(int i, string var) {
			
			// Actualizo los Resgistros
			// Aunque no se si esta parte pasa aqui
			while (!this->R[i].empty()) {
   		
    			registerChanged(*( R[i].begin() ), i);
    			this->R[i].erase(this->R[i].begin());
  			}
  			this->R[i].insert( var );

  			// Actualizo las variables
  			addAvailableAtR(var, i, 0);

		}	
		
		// LD Ri, y  ( x := y )
		void copyLoad(int i, string varY, string varX) {
			this->load(i, varY);

			this->R[i].insert(varX);

			addAvailableAtR(varX, i, 1);
		}

		
		// ST R, x
		void store(string var, int i) {
			addAvailableAtR(var, i, 0);
		}

		// OPP Ri, Rj, Rk
		// En realidad solo importa el R destion y la variable destino
		void operation(int i, string var) {

			// Actualizo los Resgistros
			while (!this->R[i].empty()) {
   		
    			registerChanged(*( R[i].begin() ), i);
    			this->R[i].erase(this->R[i].begin());
  			}
  			this->R[i].insert( var );

  			// variable recien calculada solo puede estar en ese registro
  			addAvailableAtR(var, i, 1);

		}


		bool isInSet(string var, set<string> s) {
			set<string>::iterator it;
			it = s.find(var);
			return it != s.end();
		}


		// Me dice si la variable esta en otra ubicación para reciclarla
		bool IsVarAvailable (int r, string var) {
			
			//Busqueda en los registros
			for (int i = 0; i < 33; ++i) {
				if ( isInSet(var, this->R[i]) && r != i) {
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
			for (int i = 0; i < 33; ++i) {
				if ( isInSet(op1, this->R[i]) ) {
					return i;
				}
			}

			// Hay registros libres?
			vector<int> freeRegs = this->emptyRegisters();
			if ( freeRegs.size() > 0 ) {
				return freeRegs.front();
			}
			
			// Puedo reciclar? solo si existe un solo valor en el registro
			// Operando 1
			for (int i = 0; i < 33; ++i) {
				if ( IsVarAvailable(i, *(this->R[i].begin()) ) && this->R[i].size() == 1 ) {
					return i;
				}
			}

			// ----------------------------------------------------------- Debo chequear que este sola? --------
			// Si voy a cambiar el valor a algo que esta en un registro
			// -> variable destino esta en algun resgistro sola? la voy a cambiar
			//Busqueda en los registros
			for (int i = 0; i < 33; ++i) {
				if ( isInSet(destino, this->R[i]) && (destino != op1 && destino != op2 ) ) {
					return i;
				}
			}

			// Aqui viene el spill!!!!
		}

		int getRegDestino (string destino, string op1, string op2) {

			int where;
			
			// Si un registro solo contiene el resultado que voy a cambiar
			for (int i = 0; i < 33; ++i) {
				if (	isInSet(destino, this->R[i]) 
						&& this->R[i].size() == 1 ) {
					return i;
				}
			}

			// Usar uno de los registros de los operandos para el resultado
			for (int i = 0; i < 33; ++i) {
				if ( (	isInSet(op1, this->R[i]) 
						|| isInSet(op2, this->R[i]) 
					 )
						&& this->R[i].size() == 1 ) {
					return i;
				}
			}

			// Hay registros libres?
			vector<int> freeRegs = this->emptyRegisters();
			if ( freeRegs.size() > 0 ) {
				where = freeRegs.front();
			}
			
			// Puedo reciclar?
			for (int i = 0; i < 33; ++i) {
				if ( IsVarAvailable(i, *(this->R[i].begin()) ) && this->R[i].size() == 1 ) {
					return i;
				}
			}

		}

		// No se si aqui va el 
		void getRegCopy(string op, string destino) {

			getRegOperand(op, "", destino);
		}


}; // Class



#endif