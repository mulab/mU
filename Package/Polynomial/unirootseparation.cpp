/** \file
\brief 一元方程实（复）根隔离
*/
#include "common.h"

namespace mU{

namespace
{

struct interval_map
{
	mpz_t p,q,r,s;
	void print(){std::cout<<"(";mpz_out_str(0,10,this->p);std::cout<<" ";mpz_out_str(0,10,this->q);std::cout<<" ";mpz_out_str(0,10,this->r);std::cout<<" ";mpz_out_str(0,10,this->s);std::cout<<")\n";};
	void print()const{std::cout<<"(";mpz_out_str(0,10,this->p);std::cout<<" ";mpz_out_str(0,10,this->q);std::cout<<" ";mpz_out_str(0,10,this->r);std::cout<<" ";mpz_out_str(0,10,this->s);std::cout<<")\n";};
};

void interval_map_init(interval_map & m)
{
	mpz_init(m.p);mpz_init(m.q);mpz_init(m.r);mpz_init(m.s);
	return ;
}

void interval_map_clear(interval_map & m)
{
	mpz_clear(m.p);mpz_clear(m.q);mpz_clear(m.r);mpz_clear(m.s);
	return ;
}

// using Horner's Rule
void uni_compose_z(poly_z & r,const poly_z & f,const poly_z & g)
{
	uint n=f.size()-1;
	poly_z poly_temp;
	r.resize(1);
	mpz_set(r[0],f[n]);
	if(n==0)return ;
	uint i=n-1;
	while(1)
	{
		copy_poly_z(poly_temp,r);
		UniMulZ(r,poly_temp,g);
		mpz_add(r[0],r[0],f[i]);
		if(i==0)break;
		i--;
	}
	return ;
}

// using Horner's Rule
void uni_compose_q(poly_q & r,const poly_q & f,const poly_q & g)
{
	uint n=f.size()-1;
	poly_q poly_temp;
	r.resize(1);
	mpq_set(r[0],f[n]);
	if(n==0)return ;
	uint i=n-1;
	while(1)
	{
		copy_poly_q(poly_temp,r);
		UniMulQ(r,poly_temp,g);
		mpq_add(r[0],r[0],f[i]);
		if(i==0)break;
		i--;
	}
	return ;
}

bool interval_q_cmp(const interval_q & x, const interval_q & y)
{
	if(mpq_cmp(x.first,y.first)<0||mpq_cmp(x.second,y.second)<0)return true;
		else return false;
}

void interval_q_canonicalize(interval_q & interval, const poly_q & f, mpq_ptr precision)
{
	std::vector<mpq_ptr> q_temp;
	if(mpq_cmp(interval.first,q_zero)*mpq_cmp(interval.second,q_zero)<=0);
	resize_q_list(q_temp,2);
	if(mpq_cmp(precision,q_zero)==0);
	resize_q_list(q_temp,0);
	return ;
}

void power_2(mpq_ptr r, mpz_ptr n)
{
	mpz_t y,z,m;
	mpz_init(y);
	mpz_init(z);
	mpz_init(m);
	mpz_set_ui(y,1);
	mpz_set_ui(z,2);
	mpz_abs(m,n);
	while(1)
	{
		if(mpz_odd_p(m))
		{
			mpz_mul(y,y,z);
			mpz_mul(z,z,z);
		}
		else
		{
			mpz_mul(z,z,z);
		}
		mpz_div_2exp(m,m,1);
		if(mpz_size(m)==0)break;
	}
	if(mpz_cmp_ui(n,0)<0)
	{
		mpq_set_num(r,z_unit);
		mpq_set_den(r,y);
	}
	else
	{
		mpq_set_num(r,y);
		mpq_set_den(r,z_unit);
	}
	mpz_clear(y);
	mpz_clear(z);
	mpz_clear(m);
	return ;
}

void UU(mpq_ptr r, const poly_z & a)
{
	//a.print();
	mpz_t q1,q2,p,p1,p2;
	uint n=a.size()-1;
	int sign=1;
	if(mpz_cmp_ui(a[n],0)<0)sign=-1;
	bool f1=true,f2=true;
	mpz_init(q1);mpz_init(q2);mpz_init(p);mpz_init(p1);mpz_init(p2);
	f1=true;
	for(uint i=0;i<n;++i)
	{
		if(mpz_cmp_ui(a[i],0)*sign>0)continue;
		f2=true;
		for(uint j=i+1;j<=n;++j)
		{
			if(mpz_cmp_ui(a[j],0)*sign<0)continue;
			mpz_abs(p1,a[i]);mpz_set_si(p1,mpz_sizeinbase(p1,2)-1);
			mpz_abs(p2,a[j]);mpz_set_si(p2,mpz_sizeinbase(p2,2)-1);
			//std::cout<<i<<",";mpz_out_str(0,10,p1);std::cout<<"\n";
			//std::cout<<j<<",";mpz_out_str(0,10,p2);std::cout<<"\n";
			mpz_sub(p,p1,p2);mpz_sub_ui(p,p,1);
			mpz_div_ui(p,p,j-i);mpz_add_ui(p,p,2);
			if(f2)
			{
				mpz_set(q2,p);f2=false;
			}
			else
			{
				if(mpz_cmp(q2,p)>0)mpz_set(q2,p);
			}
			//mpz_out_str(0,10,q2);std::cout<<"\n";
		}
		if(f1)
		{
			mpz_set(q1,q2);f1=false;
		}
		else
		{
			if(mpz_cmp(q1,q2)<0)mpz_set(q1,q2);
		}
		//mpz_out_str(0,10,q1);std::cout<<"\n";
	}
	mpz_add_ui(q1,q1,1);
	power_2(r,q1);
	mpz_clear(q1);mpz_clear(q2);mpz_clear(p);mpz_clear(p1);mpz_clear(p2);
	return ;
}

void reversal(poly_z & r,const poly_z & a)
{
	uint n=a.size()-1;
	r.resize(n+1);
	for(uint i=0;i<=n;++i)
	{
		mpz_set(r[n-i],a[i]);
	}
	r.normalize();
	return ;
}

void PLB(mpz_ptr r, const poly_z & a)
{
	mpq_t rr;
	mpq_init(rr);
	poly_z b;
	reversal(b,a);
	UU(rr,b);
	mpq_div(rr,q_unit,rr);
	mpz_set_q(r,rr);
	mpq_clear(rr);
	return ;
}

uint V(const poly_z & a)
{
	mpz_t sign;
	mpz_init(sign);
	mpz_set(sign,a[0]);
	uint result=0,n=a.size();
	for(uint i=1;i<n;++i)
	{
		if(mpz_cmp_ui(sign,0)*mpz_cmp_ui(a[i],0)<0)
		{
			mpz_set(sign,a[i]);
			++result;
		}
	}
	mpz_clear(sign);
	return result;
}

void CF(std::vector<interval_q> & intervallist, const poly_z & a, const interval_map & m, mpz_ptr bound)
{
	uint n=a.size()-1;
	uint va;
	poly_z temp;
	poly_z a1,a2;
	interval_map m1,m2;
	interval_q interval;
	mpz_t b;
	interval_map_init(m1);
	interval_map_init(m2);
	mpz_init(b);
	temp.resize(2);
	mpz_set_ui(temp[0],0);mpz_set_ui(temp[1],1);
	if(mpz_size(a[0])==0)
	{
		interval=interval_q();interval.init();
		mpq_set_num(interval.first,m.q);
		mpq_set_den(interval.first,m.s);
		mpq_canonicalize(interval.first);
		mpq_set(interval.second,interval.first);
		intervallist.push_back(interval);
		UniDivExactZ(a1,a,temp);
		CF(intervallist,a1,m,bound);
		goto CF_exit;
	}
	va=V(a);
	if(va==0)goto CF_exit;
	if(va==1)
	{
		interval=interval_q();interval.init();
		if(mpz_size(m.r)==0)
		{
			mpq_set_num(interval.first,bound);
			mpq_set_den(interval.first,z_unit);
		}
		else
		{
			mpq_set_num(interval.first,m.p);
			mpq_set_den(interval.first,m.r);
			mpq_canonicalize(interval.first);
		}
		if(mpz_size(m.s)==0)
		{
			mpq_set_num(interval.second,bound);
			mpq_set_den(interval.second,z_unit);
		}
		else
		{
			mpq_set_num(interval.second,m.q);
			mpq_set_den(interval.second,m.s);
			mpq_canonicalize(interval.second);
		}
		if(mpq_cmp(interval.first,interval.second)>0)
		{
			mpq_swap(interval.first,interval.second);
		}
		intervallist.push_back(interval);
		goto CF_exit;
	}
	PLB(b,a);
	//mpz_out_str(0,10,b);std::cout<<"\n";
	if(mpz_cmp_ui(b,1)>0)
	{
		mpz_set(temp[0],b);
		uni_compose_z(a1,a,temp);
		mpz_set(m1.p,m.p);
		mpz_set(m1.q,m.q);
		mpz_addmul(m1.q,b,m.p);
		mpz_set(m1.r,m.r);
		mpz_set(m1.s,m.s);
		mpz_addmul(m1.s,b,m.r);
	}
	else
	{
		copy_poly_z(a1,a);
		mpz_set(m1.p,m.p);
		mpz_set(m1.q,m.q);
		mpz_set(m1.r,m.r);
		mpz_set(m1.s,m.s);
	}
	mpz_set_ui(temp[0],1);
	uni_compose_z(a2,a1,temp);
	mpz_set(m2.p,m1.p);
	mpz_add(m2.q,m1.q,m1.p);
	mpz_set(m2.r,m1.r);
	mpz_add(m2.s,m1.s,m1.r);
	CF(intervallist,a2,m2,bound);
	if(V(a2)<V(a1))
	{
		reversal(a2,a1);
		copy_poly_z(a1,a2);
		uni_compose_z(a2,a1,temp);
		mpz_add(m2.p,m1.p,m1.q);
		mpz_set(m2.q,m1.q);
		mpz_add(m2.r,m1.r,m1.s);
		mpz_set(m2.s,m1.s);
		if(mpz_size(a2[0])==0)
		{
			mpz_set_ui(temp[0],0);
			UniDivExactZ(a1,a2,temp);
			copy_poly_z(a2,a1);
		}
		CF(intervallist,a2,m2,bound);
	}
CF_exit:
	mpz_clear(b);
	temp.resize(0);
	a1.resize(0);a2.resize(0);
	interval=interval_q();
	interval_map_clear(m1);
	interval_map_clear(m2);
	return ;
}

}

void clear_inverval_q_list(std::vector<interval_q> &x)
{
	uint size=x.size();
	for(uint i=0;i<size;++i)x[i].destroy();
	x.resize(0);
	return ;
}

void UniUpperBoundOfRootZ(mpz_ptr b, const poly_z & f)
{
	uint n=f.size()-1;
	static mpz_t ztemp;
	mpz_init(ztemp);
	mpz_set_ui(b,0);
	for(uint i=0;i<n;++i)
	{
		mpz_abs(ztemp,f[i]);
		if(mpz_cmp(b,ztemp)<0)mpz_set(b,ztemp);
	}
	mpz_abs(ztemp,f[n]);
	mpz_div(b,b,ztemp);
	mpz_add_ui(b,b,2);
	mpz_clear(ztemp);
	return ;
}

void UniSturmSequenceZ(std::vector<poly_z> & sturm_seq, const poly_z & f)
{
	clear_poly_z_list(sturm_seq);
	sturm_seq.resize(2);
	copy_poly_z(sturm_seq[0],f);
	poly_z_dform(sturm_seq[1],f);
	uint size=sturm_seq.size();
	while(sturm_seq[size-1].size()>1)
	{
		sturm_seq.resize(size+1);
		UniPseudoModZ(sturm_seq[size],sturm_seq[size-2],sturm_seq[size-1]);
		UniNegZ(sturm_seq[size],sturm_seq[size]);
		++size;
	}
	return ;
}

uint UniSturmChangeSignZ(std::vector<poly_z> & sturm_seq, mpz_ptr x)
{
	uint v=0;
	mpz_t p1,p2;
	mpz_init(p1);mpz_init(p2);
	UniEvalZ(p1,sturm_seq[0],x);
	uint size=sturm_seq.size();
	for(uint i=1;i<size;++i)
	{
		UniEvalZ(p2,sturm_seq[i],x);
		int c1=mpz_cmp_ui(p1,0),c2=mpz_cmp_ui(p2,0);
		if((c1*c2<0)||(c2==0))++v;
		mpz_set(p1,p2);
	}
	mpz_clear(p1);mpz_clear(p2);
	return v;
}

uint UniSturmChangeSignQ(std::vector<poly_q> & sturm_seq, mpq_ptr x)
{
	uint v=0;
	mpq_t p1,p2;
	mpq_init(p1);mpq_init(p2);
	UniEvalQ(p1,sturm_seq[0],x);
	uint size=sturm_seq.size();
	for(uint i=1;i<size;++i)
	{
		UniEvalQ(p2,sturm_seq[i],x);
		int c1=mpq_cmp_ui(p1,0,1),c2=mpq_cmp_ui(p2,0,1);
		if((c1*c2<0)||(c2==0))++v;
		mpq_set(p1,p2);
	}
	mpq_clear(p1);mpq_clear(p2);
	return v;
}

void UniRealRootSeparationZ(std::vector<interval_q> & intervallist, const poly_z & f)//f squarefree
{
	//deal with the case of degree=1

	std::vector<poly_z> sturm_seq_z;
	std::vector<poly_q> sturm_seq_q;
	std::set<interval_q> todo,remain;
	std::vector<mpz_ptr> z_temp;
	mpq_t c,fc,M;
	interval_q interval;
	poly_q fq,gq,hq;
	std::set<interval_q>::iterator iter;
	uint size,va,vb,vc;
	poly_z_to_poly_q(fq,f);
	interval.init();
	resize_z_list(z_temp,2);
	mpq_init(c);mpq_init(fc);mpq_init(M);
	UniUpperBoundOfRootZ(z_temp[0],f);
	mpz_set_ui(z_temp[1],1);
	mpq_set_num(interval.second,z_temp[0]);
	mpq_set_den(interval.second,z_temp[1]);
	mpz_neg(z_temp[0],z_temp[0]);
	mpq_set_num(interval.first,z_temp[0]);
	mpq_set_den(interval.first,z_temp[1]);
	todo.insert(interval);
	UniSturmSequenceZ(sturm_seq_z,f);
	size=sturm_seq_z.size();
	sturm_seq_q.resize(size);
	for(uint i=0;i<size;++i)poly_z_to_poly_q(sturm_seq_q[i],sturm_seq_z[i]);
	while(todo.size()>0)
	{
		iter=todo.begin();
		mpq_ptr pfirst = const_cast<mpq_ptr>(iter->first);
		mpq_ptr psecond = const_cast<mpq_ptr>(iter->second);
		va=UniSturmChangeSignQ(sturm_seq_q,pfirst);
		vb=UniSturmChangeSignQ(sturm_seq_q,psecond);
		if(va-vb==0)
		{
			todo.erase(iter);
			continue;
		}
		if(va-vb==1)
		{
			remain.insert(*iter);
			todo.erase(iter);
			continue;
		}
		mpq_add(c,iter->first,iter->second);
		mpq_div_2exp(c,c,1);
		vc=UniSturmChangeSignQ(sturm_seq_q,c);
		UniEvalQ(fc,fq,c);
		if(mpq_cmp_ui(fc,0,1)!=0)
		{
			interval=interval_q();interval.init();
			mpq_set(interval.first,iter->first);
			mpq_set(interval.second,c);
			if(va-vc==1)
			{
				remain.insert(interval);
			}
			else
			{
				todo.insert(interval);
			}
			interval=interval_q();interval.init();
			mpq_set(interval.first,c);
			mpq_set(interval.second,iter->second);
			if(vc-vb==1)
			{
				remain.insert(interval);
			}
			else
			{
				todo.insert(interval);
			}
			todo.erase(iter);
			continue;
		}
		else
		{
			interval=interval_q();interval.init();
			mpq_set(interval.first,c);
			mpq_set(interval.second,c);
			remain.insert(interval);
			hq.resize(2);mpq_set_ui(hq[1],1,1);mpq_set(hq[0],c);
			uni_compose_q(gq,fq,hq);//degree of f>=2
			mpq_abs(M,gq[2]);
			for(uint i=3;i<gq.size();++i)
			{
				mpq_abs(fc,gq[i]);
				if(mpq_cmp(M,fc)<0)mpq_set(M,fc);
			}
			mpq_abs(fc,gq[1]);
			mpq_div(M,M,fc);
			mpq_set_ui(fc,1,1);
			mpq_add(M,M,fc);
			mpq_div(M,fc,M);

			mpq_sub(fc,c,M);
			vc=UniSturmChangeSignQ(sturm_seq_q,fc);
			interval=interval_q();interval.init();
			mpq_set(interval.first,iter->first);
			mpq_set(interval.second,fc);
			if(va-vc==1)
			{
				remain.insert(interval);
			}
			else
			{
				todo.insert(interval);
			}
			mpq_add(fc,c,M);
			vc=UniSturmChangeSignQ(sturm_seq_q,c);
			interval=interval_q();interval.init();
			mpq_set(interval.first,fc);
			mpq_set(interval.second,iter->second);
			if(vc-vb==1)
			{
				remain.insert(interval);
			}
			else
			{
				todo.insert(interval);
			}
			todo.erase(iter);
			continue;
		}
	}
	size=remain.size();
	iter=remain.begin();
	for(uint i=0;i<size;++i)
	{
		intervallist.push_back(*iter);
		++iter;
	}
	clear_poly_q_list(sturm_seq_q);
	clear_poly_z_list(sturm_seq_z);
	while(todo.size()>0)
	{
		const_cast<interval_q*>(todo.begin().operator->())->destroy();
		todo.erase(todo.begin());
	}
	interval=interval_q();
	resize_z_list(z_temp,0);
	mpq_clear(c);mpq_clear(fc);mpq_clear(M);
	fq.resize(0);gq.resize(0);hq.resize(0);
	return ;
}

void UniRealRootSeparationZ_CF(std::vector<interval_q> & intervallist, const poly_z & f)
{
	poly_z a,temp;
	interval_map m;
	mpz_t bound;
	mpz_init(bound);
	interval_map_init(m);
	mpz_set_ui(m.p,1);
	mpz_set_ui(m.q,0);
	mpz_set_ui(m.r,0);
	mpz_set_ui(m.s,1);
	UniUpperBoundOfRootZ(bound,f);
	
	temp.resize(2);
	mpz_set_ui(temp[0],0);mpz_set_si(temp[1],-1);
	uni_compose_z(a,f,temp);
	CF(intervallist,a,m,bound);
	for(uint i=0;i<intervallist.size();++i)
	{
		mpq_swap(intervallist[i].first,intervallist[i].second);
		mpq_neg(intervallist[i].first,intervallist[i].first);
		mpq_neg(intervallist[i].second,intervallist[i].second);
	}

	copy_poly_z(a,f);
	CF(intervallist,a,m,bound);
	std::sort(intervallist.begin(),intervallist.end(),interval_q_cmp);
	a.resize(0);
	interval_map_clear(m);
	mpz_clear(bound);
	temp.resize(0);
	return ;
}

}
