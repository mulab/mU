/**
\file
\brief 多元多项式吴方法
\todo 正确性测试以后考虑效率问题
*/
#include "common.h"

namespace mU {

namespace{

uint n;

uint cl(const sparse_z & f)
{
	uint r;
	r=n-1;
	while(r>=0&&f.deg(r)==0)r--;
	r++;
	return r;
}

int cmp(const sparse_z & f,const sparse_z & g)
{
	uint tempf,tempg;
	tempf=cl(f);
	tempg=cl(g);
	if(tempf<tempg)return -1;
	if(tempf>tempg)return 1;
	tempf=f.deg(tempf-1);
	tempg=g.deg(tempg-1);
	if(tempf<tempg)return -1;
	if(tempf>tempg)return 1;
	return 0;
}

//be sure that f and g are normalized before this
bool equal_sparse_z(const sparse_z & f,const sparse_z & g)
{
	if(f.size()!=g.size())return 0;
	for(size_t i=0;i<f.size();i++)
	{
		if(f[i].exponents!=g[i].exponents)return 0;
		if(mpz_cmp(f[i].coe,g[i].coe)!=0)return 0;
	}
	return 1;
}

bool reduced(const sparse_z & f,const sparse_z & g)
{
	uint p;//be sure that p>0
	p=cl(g)-1;
	if(f.deg(p)<g.deg(p))return 1;
	return 0;
}

void mod(std::vector<sparse_z> & r,const std::vector<sparse_z> & f,const std::vector<sparse_z> & g)
{
	r.resize(f.size());
	for(uint i=0;i<f.size();i++)copy_sparse_z(r[i],f[i]);
	int k,n;
	n=g.size()-1;
	k=r.size()-1-n;
	if(k<0)return ;
	sparse_z q,lcp,temp;
	MultiPowZ(lcp,g[n],k+1);
	for(uint i=0;i<r.size();i++)MultiMulZ(r[i],f[i],lcp);
	do
	{
		MultiDivExactZ(q,r[n+k],g[n]);
		if(q.size()!=0)
		{
			for(int i=0;i<n;i++)
			{
				uint j=n+k-1-i;
				MultiMulZ(temp,q,g[j-k]);
				MultiSubZ(r[j],r[j],temp);
			}
		}
	} while (k--);
	q.resize(0);lcp.resize(0);temp.resize(0);
	int i;
	for(i=n-1;i>=0&&r[i].size()==0;i--);
	r.resize(i+1);
	return ;
}

void mod(sparse_z & r,const sparse_z & f,const sparse_z & g)
{
	std::vector<sparse_z> ff,gg,rr;
	uint p=cl(g)-1;
	//str("r1").print();
	to_main_variable(ff,f,p);to_main_variable(gg,g,p);
	//str("r2").print();
	mod(rr,ff,gg);
	//str("r3").print();
	from_main_variable(r,rr,p);
	//str("r4").print();
	clear_sparse_z_list(ff);clear_sparse_z_list(gg);clear_sparse_z_list(rr);
	return ;
}

void mod_ascending_set(sparse_z & r,const sparse_z & f,const std::vector<sparse_z> & polylist, const std::vector<uint> as)
{
	copy_sparse_z(r,f);
	for(int i=as.size()-1;i>=0;i--)
	{
		mod(r,r,polylist[as[i]]);
	}
	return ;
}

uint minimUm(const std::vector<sparse_z> & polylist,const std::vector<uint> & mark)
{
	uint r=0;
	for(uint i=1;i<mark.size();i++)
	{
		if(cmp(polylist[mark[r]],polylist[mark[i]])==1)r=i;
	}
	return r;
}

void BasicSetZ(std::vector<uint> & bs, const std::vector<sparse_z> & polylist)
{
	bs.resize(0);
	std::vector<uint> mark;
	mark.resize(polylist.size());
	for(size_t i=0;i<mark.size();i++)
	{
		mark[i]=i;
	}
	uint m,index;
	index=minimUm(polylist,mark);
	m=mark[index];
	bs.push_back(m);
	if(cl(polylist[m])==0)return ;
	mark.erase(mark.begin()+index);
	while(1)
	{
		for(size_t i=0;i<mark.size();i++)
		{
			if(!reduced(polylist[mark[i]],polylist[m]))
			{
				mark.erase(mark.begin()+i);
				i--;
			}
		}
		if(mark.size()==0)break;
		index=minimUm(polylist,mark);
		m=mark[index];
		bs.push_back(m);
		mark.erase(mark.begin()+index);
	}
	return ;
}

}

/**
\brief 对升列进行约化
\param r 约化结果
\param f 欲约化多项式
\param polylist 升列多项式集合
\param totalvar 变元个数
*/
void ReduceByAscendingSet(sparse_z & r,const sparse_z & f,const std::vector<sparse_z> & polylist, uint totalvar)
{
	n=totalvar;
	copy_sparse_z(r,f);
	for(int i=polylist.size()-1;i>=0;i--)
	{
		std::cout<<i<<"\n";
		mod(r,r,polylist[i]);
	}
	return ;
}

/**
\brief 吴特征列算法
\param polylist 多项式集合
\param totalvar 变元个数
\param cs 返回特征列集合
*/
void CharacterSetZ(std::vector<sparse_z> & cs, const std::vector<sparse_z> & polylist, uint totalvar)
{
	n=totalvar;

	std::vector<uint> bs;
	std::vector<sparse_z> pi;
	sparse_z r;
	uint oldsize;
	pi.resize(polylist.size());
	for(size_t i=0;i<pi.size();i++)
	{
		copy_sparse_z(pi[i],polylist[i]);
	}
	int count=0;
	while(1)
	{
		count++;
		BasicSetZ(bs,pi);
		oldsize=pi.size();
		//std::cout<<count<<"\n";
		//std::cout<<"pi******************\n";
		//for(uint i=0;i<pi.size();i++)pi[i].print();
		//std::cout<<"bs******************\n";
		//for(uint i=0;i<bs.size();i++)
		//{
		//	std::cout<<bs[i]<<" ";
		//	pi[bs[i]].print();
		//}
		for(uint i=0;i<oldsize;i++)
		{
			if((uint)(std::find(bs.begin(),bs.end(),i)-bs.begin())!=bs.size())continue;
			mod_ascending_set(r,pi[i],pi,bs);
			if(r.size()==0)continue;
			bool flag=1;
			for(uint j=0;j<pi.size();j++)
			{
				if(equal_sparse_z(r,pi[j]))
				{
					flag=0;break;
				}
			}
			if(flag)
			{
				pi.push_back(r);
				r=sparse_z();
			}
		}
		if(pi.size()==oldsize)break;
	}
	cs.resize(0);
	for(uint i=0;i<bs.size();i++)
	{
		cs.push_back(pi[bs[i]]);
		pi[bs[i]]=sparse_z();
	}
	r.resize(0);
	clear_sparse_z_list(pi);
}

}
