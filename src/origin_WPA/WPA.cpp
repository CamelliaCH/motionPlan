#include "WPA.h"

//�Ȱ�����ܹ�����

WPA::WPA()
{

}

WPA::~WPA()
{

}

//params: num:�ǵ����� dim:�ǵ�ά�� max_Bound/min_Bound:ÿһά��Ӧ�����Сֵ������Ĭ�ϲ�ͬά��֮�����һ��
//problem_Type:��ǰ��������Ӧ������ gen_Num:����������
void WPA::process(int num, int dim, double max_Bound, double min_Bound,int problem_Type,int gen_Num,int scout_Num,double &final_Res)
{
	head_Wolf.clear();
	wolves.clear();
	srand((int)time(NULL));
	rand();
	wolf_Num = num;
	wolf_Dim = dim;
	max_Val = max_Bound;
	min_Val = min_Bound;
	prob_Ind = problem_Type;
	scout_Times = scout_Num;//���ߴ���
	scout_Step = 0.01*max_Bound;//���ߵĲ���,���ﲻͬά���²���������ͬ  0.05
	rush_Step = 0.05*max_Bound;//��Ϯʱ�Ĳ���    0.1
	atk_Step = 0.025*max_Bound;//Χ��ʱ�Ĳ���   0.025
	d_Near = 0.08*max_Bound;//�ɱ�Ϯת��Χ�����ж���ֵ������ο�matlab�����ö�ֵ����û�а�����������  0.08
	int dir_Factor = 10;//��������  10
	double beita = 5;//Ⱥ����±������� 5
    srand((int)time(NULL));
    rand();
	param_H = dir_Factor + rand() % dir_Factor;//������Ϊ[dir_Factor,2*dir_Factor)֮�������
	param_R = (1.0 + rand() / (double)RAND_MAX) * wolf_Num * 0.5 / beita;
	init_Wolf();//��ʼ����Ⱥ
	pick_Head();//ѡ��ͷ��
	bool flag = false;
	//debug
	/*
	for (int i = 0; i < wolf_Num; i++)
	{
		if(fabs(wolf_Fit[i]-cal_Fitness(wolves[i]))>1e-5)
		    flag=true;
	}
	if (flag)
		cout << "at the beginning,error\n";
	else
		cout << "at the beginning,no error!\n";
	 */
	flag = false;
	vector<double> gen_Result;//�洢ÿһ�ε���������Ž��
	for (int i = 0; i < gen_Num; i++)
	{	
		/*
		for (int ind = 0; ind < wolf_Num; ind++)//������Ⱥ��Ӧ��
			wolf_Fit[ind] = cal_Fitness(wolves[ind]);
		*/
		ac_Wolfscout();//ac=action ������Ϊ
		ac_Summon();//�ٻ���Χ����Ϊ
		ac_Quarryment();//Ⱥ����£�ǿ������
		gen_Result.push_back(lead_Val);
		//std::cout << "current generation,the best result:" << lead_Val << endl;
	}
	final_Res = -1*lead_Val;
}


//����ֱ������򵥵�������ɷ���
//��Ĭ��ÿһά�ȵ�������һ��
void WPA::init_Wolf()
{
	for(int i=0;i<wolf_Num;i++)
	    wolf_Fit.push_back(0.0);
	MatrixXd wolf_Pack(wolf_Num, wolf_Dim);
	//�������Ҫ�޸ģ�ֱ���������
	srand((int)time(NULL));
	rand();
	for (int i = 0; i < wolf_Num; i++)
	{
		for (int j = 0; j < wolf_Dim; j++)
			wolf_Pack(i, j) =min_Val +(max_Val-min_Val)*rand() / (double)(RAND_MAX);
	}
	//������Ⱥ
	for (int i = 0; i < wolf_Num; i++)
		wolves.push_back(wolf_Pack.block(i, 0, 1, wolf_Dim));
}


void WPA::pick_Head()
{
	head_Wolf.clear();
	//������Ⱥ��Ӧ�Ȳ�ѡ��ͷ��
    srand((int)time(NULL));
    rand();
	vector<int> wolf_Candidate;
	double cur_Max = cal_Fitness(wolves[0]);
	for (int i = 0; i < wolf_Num; i++)
	{
		wolf_Fit[i] = cal_Fitness(wolves[i]);
		if (wolf_Fit[i] > cur_Max)
			cur_Max = wolf_Fit[i];
	}
	lead_Val = cur_Max;
	for (int i = 0; i < wolf_Num; i++)
		if (fabs(wolf_Fit[i] - cur_Max) < 1e-6)
			wolf_Candidate.push_back(i);
	int candi_Num = wolf_Candidate.size();
	if (candi_Num == 1)
		head_Wolf.push_back(wolf_Candidate[0]);
	else
	{
        srand((int)time(NULL));
        rand();
		int pick_Ind = rand() % candi_Num;
		head_Wolf.push_back(wolf_Candidate[pick_Ind]);
	}
	lead_Val = wolf_Fit[head_Wolf[0]];
}

//���������ǲο���Ӧ��matlab�����д�������ԭʼ��������������һЩ����
void WPA::ac_Wolfscout()
{
	int Cnt = 0;
	MatrixXd step_Stan = MatrixXd::Ones(1, wolf_Dim);
	while (Cnt < scout_Times)
	{
	    /*  //debug
	    cout<<"each time:"<<lead_Val<<endl;
	    cout<<"wolf:"<<wolves[head_Wolf[0]]<<endl;
	    cout<<"fitness:"<<cal_Fitness(wolves[head_Wolf[0]])<<endl;
	     */
		for (int i = 0; i < wolf_Num; i++)//����������Ƕ������˹�̽��,��û����ԭʼ����һ����һ��S_num
		{
			if (wolf_Fit[i] > lead_Val)
			{
				head_Wolf[0] = i;
				lead_Val = wolf_Fit[i];
				//�о���������������Ƿ��ж�ƥ�Ǵ���ͷ�ǣ�ת���ɶ�ͷ�ǵ�ģʽ
			}
			else
			{//̽����h�������������
				MatrixXd scout_Try(param_H, wolf_Dim);
				double* scout_Res = new double[param_H];
				double cur_Max = wolf_Fit[i]-1;
				for (int j = 0; j < param_H; j++)//����������������
				{
					scout_Try.block(j, 0, 1, wolf_Dim) = wolves[i] + scout_Step* sin((double)2.0 * SELF_PI * j / param_H) * step_Stan;
					scout_Res[j] = cal_Fitness(scout_Try.block(j, 0, 1, wolf_Dim));
					if (scout_Res[j] > cur_Max)
						cur_Max = scout_Res[j];
				}
				if (cur_Max <= wolf_Fit[i])//���������������ڴ�ǰ��ֵ���򲻸���
					continue;
				vector<int> candi_Dir;
				for (int j = 0; j < param_H; j++)
				{
					if (fabs(scout_Res[j] - cur_Max) < 1e-6)
						candi_Dir.push_back(j);
				}
				int candi_Num = candi_Dir.size();
				/*
				if (candi_Num == 0)
					continue;
				*/
				if (candi_Num > 1)//����ж�����ֵ�����ѡһ��
				{
					int pick_Ind = rand() % candi_Num;
					wolves[i] = scout_Try.block(candi_Dir[pick_Ind], 0, 1, wolf_Dim);
				}
				else
					wolves[i] = scout_Try.block(candi_Dir[0], 0, 1, wolf_Dim);
				wolf_Fit[i] = cur_Max;//���µ�ǰ�Ƕ�Ӧ����Ӧ��
				delete[] scout_Res;
			}
			if (wolf_Fit[i] > lead_Val)
			{
				head_Wolf[0] = i;
				lead_Val = wolf_Fit[i];
				//�о���������������Ƿ��ж�ƥ�Ǵ���ͷ�ǣ�ת���ɶ�ͷ�ǵ�ģʽ
			}
		}
		Cnt++;
	}
}

void WPA::ac_Summon()
{
	for (int i = 0; i < wolf_Num; i++)
	{
		if (i == head_Wolf[0])
			continue;
		double cur_Dist = cal_Dist(wolves[i], wolves[head_Wolf[0]],0);
		if (cur_Dist > d_Near)//��Ϯ
		{
			for (int j = 0; j < wolf_Dim; j++)
			{
				//���Ķ�Ӧ���ֵ�Ч����������ÿһά�ı�Ϯ�����ľ���ֵ��ȷ���ģ���������ͷ�ǵľ��Ծ����Ӱ��
				if (wolves[i](0, j) > wolves[head_Wolf[0]](0, j))
					wolves[i](0, j) -= rush_Step;
				else
					wolves[i](0, j) += rush_Step;
			}
			wolf_Fit[i] = cal_Fitness(wolves[i]);
			if (wolf_Fit[i] > lead_Val)//����ͷ��Ũ�����Ϊ�µ�ͷ��
			{
				head_Wolf[0] = i;
				lead_Val = wolf_Fit[i];
			}
		}
		else//Χ��
		{
			ac_Beleaguer(i);
		}
	}
}


//�о��ܶಽ���������ɾ�̬��ɶ�̬���ɵ��������;���������
void WPA::ac_Beleaguer(int wolf_Ind)//Χ��
{   //untested
    srand((int)time(NULL));
    rand();
	double r = 1 - 2.0 * rand() / (double)RAND_MAX;//-1~1֮��������
	for (int i = 0; i < wolf_Dim; i++)
	{//���ݵ�ǰά���µľ����֮ǰ�Ĺ�������ϵ������ͬȷ������
		//����������r���Է����Ҳ���Է����
		wolves[wolf_Ind](0, i) = wolves[wolf_Ind](0, i) + r * atk_Step * fabs(wolves[head_Wolf[0]](0, i) - wolves[wolf_Ind](0, i));
	}
	wolf_Fit[wolf_Ind] = cal_Fitness(wolves[wolf_Ind]);
	if (wolf_Fit[wolf_Ind] > lead_Val)//���Χ�������Ӧ�ȴ���ͷ�ǣ������
	{
		lead_Val = wolf_Fit[wolf_Ind];
		head_Wolf[0] = wolf_Ind;
	}
}

void WPA::ac_Quarryment()
{//seems no use
	int* index_Set = new int[wolf_Num];//���ڻ�ȡ����ǰ�������Ķ�Ӧ��ϵ
	double* new_Fit = new double[wolf_Num];//���뵱ǰ�ĸ��ǵ���Ӧ��
	for (int i = 0; i < wolf_Num; i++)
	{
		index_Set[i] = i;
		new_Fit[i] = wolf_Fit[i];
	}
	for (int i = 0; i < (wolf_Num-1); i++)//��С��������
	{
		for (int j = i+1; j < wolf_Num; j++)
		{
			if (new_Fit[i] > new_Fit[j])
			{
				double tmp_Fit = new_Fit[i];
				new_Fit[i] = new_Fit[j];
				new_Fit[j] = tmp_Fit;

				int tmp = index_Set[i];
				index_Set[i] = index_Set[j];
				index_Set[j] = tmp;
			}
		}
	}
	vector<int> candi_Index;
	for (int i = (wolf_Num - 1); i >= 0; i--)
	{
		if (fabs(new_Fit[i] - lead_Val) < 1e-6)
			candi_Index.push_back(i);
	}
	int head_Ind;
	int candi_Num = candi_Index.size();
	if (candi_Num > 1)
	{
		int tmp = rand() % candi_Num;
		head_Ind = candi_Index[tmp];
	}
	else
		head_Ind = candi_Index[0];
	head_Wolf[0] = index_Set[head_Ind];
	for (int i = 0; i < param_R; i++)
	{
		if (i == head_Ind)
			continue;
		for (int j = 0; j < wolf_Dim; j++)
		{
			double new_Val = wolves[head_Wolf[0]](0, j) * (1 + 0.5 * (1 - 2 * rand() / (double)RAND_MAX));
			if (new_Val > max_Val)
				new_Val = max_Val;
			if (new_Val < min_Val)
				new_Val = min_Val;
			wolves[index_Set[i]](0, j) = new_Val;
		}
		wolf_Fit[index_Set[i]] = cal_Fitness(wolves[index_Set[i]]);
	}
	delete[] index_Set;
	delete[] new_Fit;
}


double WPA::cal_Fitness(MatrixXd wolf)//�����Ӧ�����ǵ���Ӧ��
{
	double fitness = 0;
	MatrixXd standard, tmp, tmp1, tmp2;
	switch (prob_Ind)
	{
	case 0://Easom
		fitness = -1 * cos(wolf(0, 0)) * cos(wolf(0, 1)) * exp(-1 * pow(wolf(0, 0) - SELF_PI, 2) - pow(wolf(0, 1) - SELF_PI, 2));
		break;
	case 1://Matyas
		fitness = 0.26 * (wolf(0, 0) * wolf(0, 0) + wolf(0, 1) * wolf(0, 1)) - 0.48 * wolf(0, 0) * wolf(0, 1);
		break;
	case 2://Rosenbrock
		standard = MatrixXd::Ones(1, wolf_Dim - 1);
		tmp1 = wolf.block(0, 1, 1, wolf_Dim - 1) - wolf.block(0, 0, 1, wolf_Dim - 1).cwiseAbs2();
		tmp1 = 100 * tmp1.cwiseAbs2();
		tmp2 = (wolf.block(0, 0, 1, wolf_Dim - 1) - standard).cwiseAbs2();
		tmp = (tmp1 + tmp2) * standard.transpose();
		fitness = tmp(0, 0);
		break;
	case 3://Colville
		fitness = 100 * pow(wolf(0, 0) * wolf(0, 0) - wolf(0, 1), 2) + pow(wolf(0, 0) - 1, 2) + pow(wolf(0, 2) - 1, 2) + 90 * pow(wolf(0, 2) * wolf(0, 2) - wolf(0, 3), 2) + 10.1 * (pow(wolf(0, 1) - 1, 2) + pow(wolf(0, 3) - 1, 2)) + 19.8 * (wolf(0, 1) - 1) * (wolf(0, 3) - 1);
		break;
	case 4://Trid6
		standard = MatrixXd::Ones(1, wolf_Dim);
		tmp1 = (wolf - standard).cwiseAbs2();
		tmp2 = wolf.block(0, 1, 1, wolf_Dim - 1)*(wolf.block(0, 0, 1, wolf_Dim - 1)).transpose();
		tmp = tmp1 * standard.transpose() - tmp2;
		fitness = tmp(0, 0);
		break;
	case 5://Stepint
		for (int x = 0; x < wolf_Dim; x++)
			fitness =fitness+ int(wolf(0, x));
		fitness += 30.0;
		break;
	case 6://Step
		for (int x = 0; x < wolf_Dim; x++)
			fitness = fitness + pow(int(wolf(0, x) + 0.5), 2);
		break;
	case 7://Sumsquares
		tmp = wolf.cwiseAbs2();
		for (int x = 0; x < wolf_Dim; x++)
			fitness = fitness + (x + 1) * tmp(0, x);
		break;
	case 8://Sphere
		standard = MatrixXd::Ones(1, wolf_Dim);
		tmp = wolf.cwiseAbs2();
		tmp = tmp * standard.transpose();
		fitness = tmp(0, 0);
		break;
	case 9://Booth
		fitness = pow(wolf(0, 0) + 2 * wolf(0, 1) - 7, 2) + pow(2 * wolf(0, 0) + wolf(0, 1) - 5, 2);
		break;
	case 10://Bohachevskyl
		fitness = pow(wolf(0, 0), 2) + 2 * pow(wolf(0, 1), 2) - 0.3 * cos(3 * SELF_PI * wolf(0, 0)) - 0.4 * cos(4 * SELF_PI * wolf(0, 1)) + 0.7;
		break;
	case 11://Rastrigin
		break;
	Default:
		break;
	}
	fitness *= -1;//Ҫ����Ӧ��Խ��Խ�ã�������������С�����Գ���-1
	return fitness;
}

double WPA::cal_Dist(MatrixXd wolf_A, MatrixXd wolf_B,int cal_Mode)
{
	double cur_Dist = 0;
	MatrixXd tmp = wolf_A - wolf_B;
	switch (cal_Mode)
	{
	case 0://ŷʽ����
		cur_Dist = tmp.squaredNorm();
		break;
	case 1:
		for (int i = 0; i < wolf_Dim; i++)
			cur_Dist = cur_Dist + fabs(tmp(0, i));
		break;
	default:
		cout << "error distance mode\n";
		break;
	}
	return cur_Dist;
}