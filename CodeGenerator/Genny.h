class Descriptor {
	public:
		std::vector <varDescriptor*> vars;
		regDescriptor *R[32];

		Descriptor()

		regDescriptor* getRegDescr(int i)

		varDescriptor* getVarDescr(string var)

		void addVarDescr(string var)

		void addtmpVarDescr(string var)

		void addVarInVar(string var)

		int getVarWeight(string var)

		int getRegScore(int i)

		void setRDscore(int i, int score)

		void addRDscore(int i, int score)

		regDescriptor* getLowestRegister()

		void addVarInR(string var, int ri, int only)

		bool isTemp(string var)

		vector<int> emptyRegisters()

		void registerChanged(string var, int ri)

		void load(int i, string var)
			
		void copy(int i, string varY, string varX)

		void store(string var, int i)

		void operation(int i, string var)

		bool isInSet(string var, set<string> s)

		bool IsVarAvailable (int r, string var)

		int getRegOperand (string op1, string op2, string destino)

		int getRegDestino (string destino, string op1, string op2)

}; // Class