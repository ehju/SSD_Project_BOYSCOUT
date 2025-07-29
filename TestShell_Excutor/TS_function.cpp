
class DummySSD {
public:
	virtual unsigned int read(int lba)=0;
	virtual void write(int lba , unsigned int data) = 0;
};

class TS_function {

};