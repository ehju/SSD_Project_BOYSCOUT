
class DummySSD {
public:
	virtual unsigned int read(int lba)=0;
	virtual void write(int lba , unsigned int data) = 0;
};

class TS_function {
public:
	TS_function(DummySSD* ssd) : ssd { ssd } {}

	unsigned int read(int lba) {
		return  ssd->read(lba);
	}
private:
	DummySSD* ssd;
};