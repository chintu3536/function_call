int A;
float F1, F2;

int main(int a, float b);

float faddfunc(float a , float b);

int iaddfunc(int a, int b);

main(int a, float b)
{
	int x1, x2, x3;
	float f1, f2, f3;
	F1 = 9.5;
	x1 = 1;
	f1 = 3.5;
	x2 = 2;
	f2 = 4.5;
	x3 = 3;
	f3 = 5.5;

	print(faddfunc(faddfunc(faddfunc(faddfunc(1.0 + faddfunc(f2, 1.0), 2.0), 3.0), 4.0) - 5.0, 5.0));
	print("\n");
	print(iaddfunc(iaddfunc(iaddfunc(iaddfunc(3 + iaddfunc(x2, 1), 2), 3), 4) - 5, 5));
	return 0;
}

faddfunc(float a , float b)
{
	float c;
	c = a;
	if(b == 0.0) return a;
	return 1.0 + faddfunc(c, b - 1.0);
}

iaddfunc(int a , int b)
{
	int c;
	c = a;
	if(b == 0) return a;
	return 1 + iaddfunc(c, b - 1);
}
