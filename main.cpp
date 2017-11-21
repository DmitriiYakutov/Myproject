#include <QCoreApplication>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <iomanip>


using namespace std;
class robot;
class element;
class joint;
class matrix;
void RotX(matrix &R, float a);
void RotY(matrix &R, float a);
void RotZ(matrix &R, float a);
void TransX(matrix &T, float d);
void TransY(matrix &T, float d);
void TransZ(matrix &T, float d);
void multiplicationFour(matrix &V, matrix &B, matrix &C);
void multiplicationVector(matrix &V, matrix &B, matrix &C);
void rezult(matrix &A);
void installRobot(robot &Robot);
void nextStep(robot &Robot);
void printMatrix(matrix &A);

//=====================MATRIX====================================
class matrix
{
public:
    float four[4][4];   //матрица 4х4
    float vector[3];    //вектор 3х1
    matrix(){}
    void setMatrixOne()
    {
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                if(i == j)
                    four[i][j] = 1;
                else
                    four[i][j] = 0;
            }
        }
        for(int j = 0; j < 3; j++)
        {
            vector[j] = 1;
        }
    }
    void setMatrixZero()
    {
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                    four[i][j] = 0;
            }
        }
        for(int j = 0; j < 3; j++)
        {
            vector[j] = 0;
        }
    }
    ~matrix(){};
};
//========================JOINT=================================
class joint
{
public:
    friend class robot;
    friend class element;
    matrix R;
    matrix T;
    joint()
    {
        move_z = 0;
        move_x = 0;
        orientation_x = 0;
        orientation_z = 0;
    }
    void setJoint(int i)
    {
        ind = i;
        if(ind == 0)
        {
            cout<<"0-евая кинематическая пара по-умолчанию:"<<endl;
            cout<<"имеет относительную ориентацию равную 0"<<endl;
            orientation_x = 0;
            move_x = 0;
            orientation_z = 0;
            move_z = 0;
        }
        else
        {
            while(type != 1 &&  type != 2)
            {
                cout<<"Введите корректный тип КП!"<<endl;
                cout<<"Введите тип КП №-"<<ind<<" (1 - вращательная, 2 - поступательная): ";
                cin>>type;
            }
            if(ind == 1)
            {
                cout<<"Введите поворот относительно Z предыдущей системы координат(рад): "<<endl;
                cout<<"(оси Х(i-1) и Х(i) параллельны): ";
                cin>>orientation_z;
                cout<<"Введите смещение по Z предыдущей системы координат(м):"<<endl;
                cout<<"(оси Х(i-1) и Х(i) совпадают): ";
                cin>>move_z;
                cout<<"Введите смещение по X предыдущей системы координат(м):"<<endl;
                cout<<"(начала О(i-1) и О(i) совпадают): ";
                cin>>move_x;
                cout<<"Введите поворот относительно X предыдущей системы координат(рад): "<<endl;
                cout<<"(оси систем координат (i-1) и (i) параллельны): ";
                cin>>orientation_x;
            }
            else
            {
                if (type == 2)
                {
                    cout<<"Введите поворот относительно Z предыдущей системы координат(рад): "<<endl;
                    cout<<"(оси Х(i-1) и Х(i) параллельны): ";
                    cin>>orientation_z;
                }
                cout<<"Введите поворот относительно X предыдущей системы координат(рад): "<<endl;
                cout<<"(оси систем координат (i-1) и (i) параллельны): ";
                cin>>orientation_x;
                if (type == 1)
                {
                    cout<<"Введите смещение по Z предыдущей системы координат(м):"<<endl;
                    cout<<"(оси Х(i-1) и Х(i) совпадают) ВОЗМОЖНО РАВНО ДЛИНЕ ЗВЕНА: ";
                    cin>>move_z;
                }
                cout<<"Введите смещение по X предыдущей системы координат(м):"<<endl;
                cout<<"(начала О(i-1) и О(i) совпадают) ВОЗМОЖНО РАВНО ДЛИНЕ ЗВЕНА ";
                cin>>move_x;
            }
        }

    }

    void getJoint()
    {

        if (type == 1)
             cout<<"Тип "<<ind<<"-ой кинематической пары - вращательный"<<endl;
        if (type == 2)
             cout<<"Тип "<<ind<<"-ой кинематической пары - поступательный"<<endl;
    }

    void move()
    {
        if(ind != 0)
        {
            if (type == 1)
            {
                cout<<"Введите угол поворота во вращательной КП  №"<<ind<<" (рад): ";
                cin>>orientation_z;
            }
            if (type == 2)
            {
                cout<<"Введите линейное перемещение в поступательной КП №"<<ind<<" (м): ";
                cout<<"НЕ БОЛЬШЕ ДЛИНЫ ЗВЕНА: ";
                cin>>move_z;
            }
        }
    }

    void getOrientation()
    {
        cout<<"ТИП "<<type<<endl;
        cout<<"Вращение по z "<<orientation_z<<endl;
        cout<<"Вращение по х "<<orientation_x<<endl;
        cout<<"Сдвиг по z "<<move_z<<endl;
        cout<<"Сдвиг по x "<<move_x<<endl;
    }

    ~joint(){}
private:
    unsigned int ind;
    int type;            //1 - вращательная КП, 2-поступательная КП
    float orientation_x; //ориентация оси КП относительно предыдущего х
    float orientation_z; //ориентация относительно предыдущего z - может быть обощенной координатой
    //обощённая координата - зависит от типа кинематической пары
    float move_z;        //может быть обобщенной координатой
    float move_x;
};
//=====================ELEMENT====================================
class element
{
public:
    friend class robot;
    friend class joint;
    element()
    {
        lenght = 0;
    }

    void setElement(int i)
    {
        ind = i;
        if(ind != 0)
        {
            cout<<"Введите длину "<<ind<<" звена робота: ";
            cin>>lenght;
        }
        else lenght = 0;
    }

    void getElement()
    {
        cout<<"Длина "<<ind<<"-ого звена: "<<lenght<<endl;
    }

    ~element(){};
private:
    unsigned int ind;
    float lenght;
};
//====================ROBOT=====================================
class robot
{
public:
    friend class element;
    friend class joint;
    matrix A;
    matrix B;
    element *Element = new element();
    joint *Joint;
    //собирается робот из звеньев и сочленений
    //robot():Joint(nullptr){}
    void setRobot(int COUNT)
    {
        Joint = new joint[COUNT];
        number = COUNT;
        for(int i = 0; i < number; i++)
        {
            Joint[i].setJoint(i);
            Element[i].setElement(i);
        }
    }

    void getRobot()
    {
        cout<<"Вы создали робота, имеющего следующие параметры:"<<endl;
        cout<<"Колличество звеньев у робота:  "<<number<<endl;
        cout<<"____________________________________________________"<<endl;
        for(int i = 0; i < number; i++)
        {
            Joint[i].getJoint();
            Element[i].getElement();
            cout<<"____________________________________________________"<<endl;
        }
    }

    void calculation()
    {
        B.setMatrixOne();
        for(int i = 0; i < number; i++)
        {
            Joint[i].R.setMatrixOne();
            Joint[i].T.setMatrixOne();
            A.setMatrixZero();
            Joint[i].move();
            RotZ(Joint[i].R, Joint[i].orientation_z);
            multiplicationFour(A, B, Joint[i].R);
            B = A;
            A.setMatrixZero();
            Joint[i].T.setMatrixOne();
            TransZ(Joint[i].T, Joint[i].move_z);
            multiplicationFour(A, B, Joint[i].T);
            B = A;
            A.setMatrixZero();
            Joint[i].T.setMatrixOne();
            TransX(Joint[i].T, Joint[i].move_x);
            multiplicationFour(A, B, Joint[i].T);
            B = A;
            A.setMatrixZero();
            Joint[i].R.setMatrixOne();
            RotX(Joint[i].R, Joint[i].orientation_x);
            multiplicationFour(A, B, Joint[i].R);
        }
    }

    ~robot()
    {
//        if (Joint != nullptr)
//        {
//            delete[] Joint;
//        }
    }
private:
     int number;
};

//#############################################################
//====================MAIN=====================================
int main()
{
    robot kuka;
    installRobot(kuka);
    nextStep(kuka);
    return 0;
}
//#################################################################
//====================Rotation X===================================
void RotX(matrix &R, float a)
{
    R.setMatrixOne();
    R.four[1][1] = cos(a);
    R.four[1][2] = -sin(a);
    R.four[2][1] = sin(a);
    R.four[2][2] = cos(a);
}
//====================Rotation Y===================================
void RotY(matrix &R, float a)
{
    R.setMatrixOne();
    R.four[0][0] = cos(a);
    R.four[0][2] = sin(a);
    R.four[2][0] = -sin(a);
    R.four[2][2] = cos(a);
}
//====================Rotation Z===================================
void RotZ(matrix &R, float a)
{
    R.setMatrixOne();
    R.four[0][0] = cos(a);
    R.four[0][1] = -sin(a);
    R.four[1][0] = sin(a);
    R.four[1][1] = cos(a);
}
//====================Transport X===================================
void TransX(matrix &T, float d)
{
    T.setMatrixOne();
    T.four[0][3] = d;
}
//====================Transport Y===================================
void TransY(matrix &T, float d)
{
    T.setMatrixOne();
    T.four[1][3] = d;
}
//====================Transport Z===================================
void TransZ(matrix &T, float d)
{
    T.setMatrixOne();
    T.four[2][3] = d;
}
//==================Multiplication of matrix=======================
void multiplicationFour(matrix &A, matrix &B, matrix &C)
{
    for(int k = 0; k < 4; k++)
    {
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                A.four[k][i] += ((B.four[k][j])*(C.four[j][i]));
            }
        }
    }
}
//==================Multiplication of vector=======================
void multiplicationVector(matrix &V, matrix &B, matrix &C)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            V.vector[i] += B.four[i][j]*C.vector[j];
        }
    }
}
//==================Rezult of calculation===========================
void rezult(matrix &A)
{
    printMatrix(A);
    for(int i = 0; i < 3; i++)
        A.vector[i] = A.four[i][3];
    cout<<"Новые координаты схвата манипулятора: x = "<<A.vector[0]<<" y = "<<A.vector[1]<<" z = "<<A.vector[2]<<endl;
}
//==================install Robot===========================
void installRobot(robot &Robot)
{
    cout<<"Введите количество звеньев в роботе (с учётом стойки): ";
    int n = 0;
    cin>>n;
    Robot.setRobot(n);
    cout<<"Готово!!! Вы можете посмотреть параметры робота или вывести решение"<<endl;
}
//==================next step (get robot or calculation)===========================
void nextStep(robot &Robot)
{

    int action = 1;
    while(action != 0)
    {
        cout<<"Введите номер дальнейшего действия: "<<endl;
        cout<<" 1 - заново перестроить робота, 2 - вывести параметры робота,"<<endl;
        cout<<"3 - решение кинематической задачи, 0 - выход"<<endl;
        cin>>action;
        system("clear");
        switch (action) {
        case 0: break;
        case 1: installRobot(Robot);
            break;
        case 2: Robot.getRobot();
            break;
        case 3:
        {
            Robot.calculation();
            rezult(Robot.A);
        }
            break;
        default: cout<<"Не верная команда! Введите правильное действие!"<<endl;
            break;
        }
    }
}

void printMatrix(matrix &A)
{
    cout<<"------------Итоговая матрица----------"<<endl;
    for(int i = 0; i < 4; i++)
    {
        cout<<"| ";
        for(int j = 0; j < 4; j++)
        {
            cout<<setw(6)<<setprecision(2)<<A.four[i][j]<<"  ";
        }
        cout<<" |"<<endl;
    }
    cout<<"____________________________________________________"<<endl;
}
