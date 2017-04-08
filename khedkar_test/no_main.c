int i, size;
int prev_number;
int number;
void main(int a);
int fibonacci (int prev_number, int number);



fibonacci (int prev_number, int number)
{
    int next_num;
    if (i==size)
    {
        print(i);
    }
    else
    {
      next_num=prev_number+number;
      prev_number=number;
      number=next_num;
      i = i+1; 
      i = fibonacci(prev_number,number); 
    }
    return 0;
}



main(int a)
{
    prev_number=0;
    number=1;
    size = 20;
    i = 1;
    fibonacci (prev_number,number);
    return ;
}


