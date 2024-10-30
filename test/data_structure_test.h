template <typename T>
struct MyType{
	int x;
	
	void hi();
};

template <typename T>
void MyType<T>::hi() {
	return;
}