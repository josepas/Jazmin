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

		void varAvailableAtR(string var, int i, int only) {
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
  			varAvailableAtR(var, i, 0);

		}	
		
		// LD Ri, y  ( x := y )
		void copyLoad(int i, string varY, string varX) {
			this->load(i, varY);

			this->R[i].insert(varX);

			varAvailableAtR(varX, i, 0);
		}

		


		// ST R, x
		void store(string var, int i) {
			varAvailableAtR(var, i, 0);
		}

		// ADD Ri, Rj, Rk
		// En realidad solo importa el R destion y la variable destino
		void operation(int i, string var) {

			// Actualizo los Resgistros
			while (!this->R[i].empty()) {
   		
    			registerChanged(*( R[i].begin() ), i);
    			this->R[i].erase(this->R[i].begin());
  			}
  			this->R[i].insert( var );

  			// variable recien calculada solo puede estar en ese registro
  			varAvailableAtR(var, i, 1);

		}





};



#endif