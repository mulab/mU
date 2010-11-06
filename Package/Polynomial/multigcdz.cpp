/**
\file
\brief 多元多项式GCD in Z[X]
*/
#include "common.h"

namespace mU {

namespace{

void SolveVandermondeT(std::vector<mpz_ptr> & x, const std::vector<mpz_ptr> & k, const std::vector<mpz_ptr> & w, uint n)
{
	if(n==0)return ;
	std::vector<mpq_ptr> xq,wq;
	poly_z q,q1,q_temp;
	mpz_t ztemp,c,unit;
	mpq_t qtemp;
	mpz_init(ztemp);
	mpz_init(c);
	mpz_init(unit);
	mpz_set_ui(unit,1);
	mpq_init(qtemp);

	q1.resize(2);
	mpz_set_ui(q1[1],1);
	q.resize(2);
	mpz_set_ui(q[1],1);
	mpz_neg(q[0],k[0]);
	for(uint i=1;i<n;++i)
	{
		mpz_neg(q1[0],k[i]);
		UniMulZ(q_temp,q,q1);
		q=q_temp;q_temp=poly_z();
	}
	resize_q_list(xq,n);
	resize_q_list(wq,n);
	for(uint i=0;i<n;++i)
	{
		mpq_set_num(wq[i],w[i]);
		mpq_set_den(wq[i],unit);
	}
	resize_z_list(x,n);
	for(uint i=0;i<n;++i)
	{
		q1.resize(2);
		mpz_set_ui(q1[1],1);
		mpz_neg(q1[0],k[i]);
		UniDivExactZ(q_temp,q,q1);
		UniEvalZ(c,q_temp,k[i]);
		mpz_mul(c,c,k[i]);
		mpq_set_ui(xq[i],0,1);
		for(uint j=0;j<n;++j)
		{
			mpq_set_den(qtemp,c);
			mpq_set_num(qtemp,q_temp[j]);
			mpq_canonicalize(qtemp);
			mpq_mul(qtemp,qtemp,wq[j]);
			mpq_add(xq[i],xq[i],qtemp);
		}
		mpq_get_num(x[i],xq[i]);
	}
	resize_q_list(xq,0);
	resize_q_list(wq,0);
	q.resize(0);q1.resize(0);q_temp.resize(0);
	mpz_clear(ztemp);mpz_clear(c);mpz_clear(unit);
	mpq_clear(qtemp);
	return ;
}

}

void MultiGcdZ_Heuristic(sparse_z & r, const sparse_z & f, const sparse_z & g, uint n)
{
	return ;
}

void MultiGcdZ_Dense_Interpolation(sparse_z & r, const sparse_z & f, const sparse_z & g, uint n)
{
	mpz_t contf,contg,a,b,q,p,limit,bp,p2,c;
	sparse_z _f,_g,h,fp,gp,hp;
	uint m,m1,size;
	mpz_init(contf);mpz_init(contg);mpz_init(a);mpz_init(b);mpz_init(q);mpz_init(p);mpz_init(limit);mpz_init(bp);mpz_init(p2);mpz_init(c);
	MultiContPPZ(contf,_f,f);MultiContPPZ(contg,_g,g);mpz_gcd(a,contf,contg);
	mpz_set(contf,_f[_f.size()-1].coe);mpz_set(contg,_g[_g.size()-1].coe);mpz_gcd(b,contf,contg);
	mpz_set_ui(q,1);m=std::min<uint>(_f.deg(),_g.deg())+1;
	
	MultiMaxNormZ(contf,_f);MultiMaxNormZ(contg,_g);mpz_set(limit,mpz_cmp(contf,contg)<0?contf:contg);
	mpz_abs(contf,b);mpz_mul(limit,limit,contf);mpz_mul_2exp(limit,limit,m);

	mpz_set_ui(p,m*2);
	mpz_div_2exp(p2,p,1);
	while(1)
	{
		mpz_nextprime(p,p);
		if(mpz_divisible_p(b,p)!=0)continue;
		if(mpz_divisible_p(q,p)!=0)continue;
		MultiPolynomialMod(fp,_f,p);MultiPolynomialMod(gp,_g,p);
		mpz_mod(bp,b,p);
		if(mpz_cmp(bp,p2)>0)mpz_sub(bp,bp,p);
		MultiGcdZp_Dense_Interpolation(hp,fp,gp,n,p);
		m1=hp.deg();
		if(m1>m)continue;
		size=hp.size();
		mpz_invert(c,hp[size-1].coe,p);
		for(uint i=0;i<size;++i)
		{
			mpz_mul(hp[i].coe,hp[i].coe,c);
			mpz_mul(hp[i].coe,hp[i].coe,bp);
		}
		if(m>m1)
		{
			mpz_set(q,p);
			copy_sparse_z(h,hp);
			m=m1;
		}
		else
		{
			mpz_gcdext(c,contf,contg,q,p);
			mpz_mul(contf,contf,q);
			mpz_mul(contg,contg,p);
			MultiMulZ(h,h,contg);
			MultiMulZ(hp,hp,contf);
			MultiAddZ(h,h,hp);
			mpz_mul(q,q,p);
			MultiPolynomialMod(h,h,q);
		}
		if(m==0)
		{
			r.resize(1);
			mpz_set(r[0].coe,a);
			r[0].exponents.resize(n);
			for(uint i=0;i<n;++i)r[0].exponents[i]=0;
			_f.resize(0);_g.resize(0);h.resize(0);hp.resize(0);fp.resize(0);gp.resize(0);
			mpz_clear(contf);mpz_clear(contg);mpz_clear(a);mpz_clear(b);mpz_clear(q);mpz_clear(p);mpz_clear(limit);mpz_clear(bp);mpz_clear(p2);mpz_clear(c);
			return ;
		}
		if(mpz_cmp(q,limit)>0)
		{
			MultiPPZ(hp,h);
			if(MultiDivisibleZ(_f,hp)&&MultiDivisibleZ(_g,hp))
			{
				MultiMulZ(r,hp,a);
				_f.resize(0);_g.resize(0);h.resize(0);hp.resize(0);fp.resize(0);gp.resize(0);
				mpz_clear(contf);mpz_clear(contg);mpz_clear(a);mpz_clear(b);mpz_clear(q);mpz_clear(p);mpz_clear(limit);mpz_clear(bp);mpz_clear(p2);mpz_clear(c);
				return ;
			}
		}
	}
	return ;
}

void MultiGcdZ_sparse2(sparse_z & r, const sparse_z & skel, const sparse_z & l, const sparse_z & f, const sparse_z & g, uint Tk, uint k, uint D)
{
	if(k==0)
	{
		poly_z ff,gg,rr;
		sparse_z_to_poly_z(ff,f);sparse_z_to_poly_z(gg,g);
		UniGcdZ(rr,ff,gg);
		mpz_t c;mpz_init(c);
		mpz_set(c,l[0].coe);
		for(uint i=0;i<rr.size()-1;++i)
		{
			mpz_mul(rr[i],rr[i],c);
			mpz_div(rr[i],rr[i],rr[rr.size()-1]);
		}
		mpz_set(rr[rr.size()-1],c);
		poly_z_to_sparse_z(r,rr,1,0);
		mpz_clear(c);
		ff.resize(0);gg.resize(0);rr.resize(0);
		return ;
	}
	mpz_t low,high,c,lc;
	std::vector<mpz_ptr> y,x,vandemonde;
	std::vector< std::vector< mpz_ptr > > H(D+1);
	for(uint i=0;i<=D;++i)resize_z_list(H[i],Tk);
	sparse_z lv,fv,gv;
	std::vector<sparse_z> skel_c;
	poly_z ww,ff,gg;
	mpz_init(high);mpz_init(low);mpz_init(c);mpz_init(lc);
	mpz_set_ui(high,Tk*Tk*Tk);mpz_neg(low,high);
	resize_z_list(y,k);
	resize_z_list(x,k);
	to_main_variable(skel_c,skel,k);
	for(uint i=skel_c.size();i<=D;++i)skel_c.push_back(sparse_z());
	while(1)
	{
		for(uint i=0;i<k;++i)
		{
			do 
			{
				random::randominteger(y[i],low,high);
			} while (mpz_cmp_ui(y[i],0)==0);
			mpz_set_ui(x[i],1);
		}
		for(uint i=0;i<Tk;++i)
		{
			copy_sparse_z(lv,l);
			copy_sparse_z(fv,f);
			copy_sparse_z(gv,g);
			for(uint j=0;j<k;++j)
			{
				mpz_mul(x[j],x[j],y[j]);
				eval_in_one_variable_z(lv,lv,x[j],0);
				eval_in_one_variable_z(fv,fv,x[j],0);
				eval_in_one_variable_z(gv,gv,x[j],0);
			}
			sparse_z_to_poly_z(ff,fv);
			sparse_z_to_poly_z(gg,gv);
			UniGcdZ(ww,ff,gg);
			mpz_set(c,lv[0].coe);
			mpz_set(lc,ww[ww.size()-1]);
			for(uint j=0;j<ww.size();++j)
			{
				mpz_mul(ww[j],ww[j],c);
				mpz_div(ww[j],ww[j],lc);
				mpz_set(H[j][i],ww[j]);
			}
			for(uint j=ww.size();j<=D;++j)mpz_set_ui(H[j][i],0);
		}
		for(uint j=0;j<=D;++j)
		{
			uint size=skel_c[j].size();
			resize_z_list(x,size);
			for(uint jj=0;jj<size;++jj)
			{
				mpz_set_ui(x[jj],1);
				for(uint jjj=0;jjj<k;++jjj)
				{
					mpz_pow_ui(c,y[jjj],skel_c[j][jj].exponents[jjj]);
					mpz_mul(x[jj],x[jj],c);
				}
				for(uint jjj=0;jjj<jj;++jjj)
				{
					if(mpz_cmp(x[jjj],x[jj])==0)goto MultiGcdZ_sparse2_continue_while;
				}
			}
			SolveVandermondeT(vandemonde,x,H[j],size);
			//for(uint test=0;test<size;++test){mpz_out_str(0,10,x[test]);std::cout<<" ";}
			//std::cout<<"\n";
			//for(uint test=0;test<size;++test){mpz_out_str(0,10,H[j][test]);std::cout<<" ";}
			//std::cout<<"\n";
			//for(uint test=0;test<size;++test){mpz_out_str(0,10,vandemonde[test]);std::cout<<" ";}
			//std::cout<<"\n";
			for(uint jj=0;jj<size;++jj)mpz_set(skel_c[j][jj].coe,vandemonde[jj]);
		}
		from_main_variable(r,skel_c,k);
		break;
MultiGcdZ_sparse2_continue_while:;
	}
MultiGcdZ_sparse2_exit:
	mpz_clear(high);mpz_clear(low);mpz_clear(c);mpz_clear(lc);
	resize_z_list(y,0);resize_z_list(x,0);resize_z_list(vandemonde,0);
	for(uint i=0;i<=D;++i)resize_z_list(H[i],0);
	lv.resize(0);fv.resize(0);gv.resize(0);
	clear_sparse_z_list(skel_c);
	ww.resize(0);ff.resize(0);gg.resize(0);
	return ;
}

void MultiGcdZ_Sparse1(sparse_z & r, const sparse_z & l, const sparse_z & f, const sparse_z & g, uint k, uint D)
{
	if(k==0)
	{
		poly_z ff,gg,rr;
		sparse_z_to_poly_z(ff,f);sparse_z_to_poly_z(gg,g);
		UniGcdZ(rr,ff,gg);
		mpz_t c;mpz_init(c);
		mpz_set(c,l[0].coe);
		for(uint i=0;i<rr.size()-1;++i)
		{
			mpz_mul(rr[i],rr[i],c);
			mpz_div(rr[i],rr[i],rr[rr.size()-1]);
		}
		mpz_set(rr[rr.size()-1],c);
		poly_z_to_sparse_z(r,rr,1,0);
		mpz_clear(c);
		ff.resize(0);gg.resize(0);rr.resize(0);
		return ;
	}
	uint d=std::min<uint>(f.deg(k-1),g.deg(k-1))+l.deg(k-1)+1;//plus 1!
	uint Tk,size;
	mpz_t high,low,xk,y;
	sparse_z I,lv,fv,gv,w;
	sparse_z q;
	poly_z q_one_v;
	std::vector<mpz_ptr> vlist,ztemp;
	std::vector<poly_z> poly_temp;
	std::vector<sparse_z> clist,clist_incre,sparse_temp;
	mpz_init(high);mpz_init(low);mpz_init(xk);mpz_init(y);
	resize_z_list(ztemp,2);
	sparse_temp.resize(2);
	poly_temp.resize(2);

	mpz_set_ui(high,d*d*d);
	mpz_neg(low,high);
	random::randominteger(xk,low,high);
	resize_z_list(vlist,1);
	mpz_set(vlist[0],xk);
	eval_in_one_variable_z(lv,l,xk,k-1);
	eval_in_one_variable_z(fv,f,xk,k-1);
	eval_in_one_variable_z(gv,g,xk,k-1);
	//std::cout<<"[";mpz_out_str(0,10,xk);std::cout<<"]\n";
	MultiGcdZ_Sparse1(I,lv,fv,gv,k-1,D);
	if(I.size()==0)
	{
		r.resize(0);
		goto MultiGcdZ_Sparse1_exit;
	}
	to_main_variable(clist,I,k-1);
	Tk=0;
	for(uint i=0;i<clist.size();++i)
	{
		size=clist[i].size();
		if(Tk<size)Tk=size;
		for(uint j=0;j<size;++j)clist[i][j].exponents.push_back(0);
	}
	size=clist.size();
	for(uint i=size;i<=D;++i)
	{
		clist.push_back(sparse_z());
	}
	//I.print();
	q.resize(2);mpz_set_ui(q[1].coe,1);mpz_neg(q[0].coe,xk);q[1].exponents.resize(k);q[0].exponents.resize(k);
	for(uint i=0;i<k;++i)
	{
		q[1].exponents[i]=0;
		q[0].exponents[i]=0;
	}
	q[1].exponents[k-1]=1;
	q_one_v.resize(2);mpz_set_ui(q_one_v[1],1);mpz_neg(q_one_v[0],xk);
	for(uint i=0;i<d;++i)
	{
		while(1)
		{
			uint j;
			random::randominteger(y,low,high);
			for(j=0;j<=i;++j)
			{
				if(mpz_cmp(vlist[j],y)==0)break;
			}
			if(j>i)break;
		}
		resize_z_list(vlist,i+2);
		mpz_set(vlist[i+1],y);
		eval_in_one_variable_z(lv,l,y,k-1);
		eval_in_one_variable_z(fv,f,y,k-1);
		eval_in_one_variable_z(gv,g,y,k-1);
		MultiGcdZ_sparse2(w,I,lv,fv,gv,Tk,k-1,D);

		//std::cout<<"[";mpz_out_str(0,10,y);std::cout<<"]\n";
		//w.print();

		//begin to interpolate
		to_main_variable(clist_incre,w,k-1);
		for(uint j=0;j<clist_incre.size();++j)
		{
			size=clist_incre[j].size();
			for(uint jj=0;jj<size;++jj)clist_incre[j][jj].exponents.push_back(0);
		}
		UniEvalZ(ztemp[0],q_one_v,y);
		size=clist_incre.size();
		for(uint j=0;j<size;++j)
		{
			eval_in_one_variable_z(sparse_temp[0],clist[j],y,k-1);
			for(uint jj=0;jj<sparse_temp[0].size();++jj)sparse_temp[0][jj].exponents.push_back(0);
			MultiSubZ(sparse_temp[0],clist_incre[j],sparse_temp[0]);
			MultiMulZ(sparse_temp[1],sparse_temp[0],q);
			for(uint jj=0;jj<sparse_temp[1].size();++jj)
			{
				mpz_div(sparse_temp[1][jj].coe,sparse_temp[1][jj].coe,ztemp[0]);
			}
			MultiAddZ(clist[j],clist[j],sparse_temp[1]);
		}
		sparse_temp[0].resize(2);mpz_set_ui(sparse_temp[0][1].coe,1);mpz_neg(sparse_temp[0][0].coe,y);
		sparse_temp[0][1].exponents.resize(k);sparse_temp[0][0].exponents.resize(k);
		for(uint i=0;i<k;++i)
		{
			sparse_temp[0][1].exponents[i]=0;
			sparse_temp[0][0].exponents[i]=0;
		}
		sparse_temp[0][1].exponents[k-1]=1;
		MultiMulZ(sparse_temp[1],sparse_temp[0],q);
		q=sparse_temp[1];sparse_temp[1]=sparse_z();
		poly_temp[0].resize(2);mpz_set_ui(poly_temp[0][1],1);mpz_neg(poly_temp[0][0],y);
		UniMulZ(poly_temp[1],poly_temp[0],q_one_v);
		q_one_v=poly_temp[1];poly_temp[1]=poly_z();
	}
	from_main_variable(r,clist,k);
	//std::cout<<"Interpolation result: ";r.print();
	if(MultiDivisibleZ(f,r)&&MultiDivisibleZ(g,r))
	{

	}
	else
	{
		r.resize(0);
	}
MultiGcdZ_Sparse1_exit:
	mpz_clear(high);mpz_clear(low);mpz_clear(xk);mpz_clear(y);
	I.resize(0);lv.resize(0);fv.resize(0);gv.resize(0);w.resize(0);
	q.resize(0);
	q_one_v.resize(0);
	resize_z_list(ztemp,0);
	resize_z_list(vlist,0);
	clear_poly_z_list(poly_temp);
	clear_sparse_z_list(sparse_temp);
	clear_sparse_z_list(clist);
	clear_sparse_z_list(clist_incre);
	return ;
}

void MultiGcdZ_Sparse(sparse_z & r, const sparse_z & f, const sparse_z & g, uint n)
{
	if(n==1)
	{
		poly_z ff,gg,hh;
		sparse_z_to_poly_z(ff,f);
		sparse_z_to_poly_z(gg,g);
		UniGcdZ(hh,ff,gg);
		poly_z_to_sparse_z(r,hh,1,0);
		return ;
	}
	sparse_z l,f1,g1,a,h;
	std::vector<sparse_z> fc,gc;
	uint D=std::min<uint>(f.deg(n-1),g.deg(n-1));
	to_main_variable(fc,f,n-1);
	to_main_variable(gc,g,n-1);
	copy_sparse_z(f1,fc[0]);
	for(uint i=1;i<fc.size();++i)
	{
		MultiGcdZ_Sparse(f1,f1,fc[i],n-1);
	}
	copy_sparse_z(g1,gc[0]);
	for(uint i=1;i<gc.size();++i)
	{
		MultiGcdZ_Sparse(g1,g1,gc[i],n-1);
	}
	MultiGcdZ_Sparse(a,f1,g1,n-1);
	for(uint i=0;i<a.size();++i)a[i].exponents.push_back(0);
	for(uint i=0;i<fc.size();++i)
	{
		MultiDivExactZ(fc[i],fc[i],f1);
	}
	for(uint i=0;i<gc.size();++i)
	{
		MultiDivExactZ(gc[i],gc[i],g1);
	}
	MultiGcdZ_Sparse(l,fc[fc.size()-1],gc[gc.size()-1],n-1);
	for(uint i=0;i<fc.size();++i)MultiMulZ(fc[i],fc[i],l);
	for(uint i=0;i<gc.size();++i)MultiMulZ(gc[i],gc[i],l);
	from_main_variable(f1,fc,n-1);
	from_main_variable(g1,gc,n-1);

	MultiGcdZ_Sparse1(h,l,f1,g1,n-1,D);
	to_main_variable(fc,h,n-1);
	copy_sparse_z(f1,fc[0]);
	for(uint i=1;i<fc.size();++i)
	{
		MultiGcdZ_Sparse(f1,f1,fc[i],n-1);
	}
	for(uint i=0;i<fc.size();++i)MultiDivExactZ(fc[i],fc[i],f1);
	from_main_variable(h,fc,n-1);
	MultiMulZ(r,a,h);
	l.resize(0);f1.resize(0);g1.resize(0);a.resize(0);h.resize(0);
	clear_sparse_z_list(fc);
	clear_sparse_z_list(gc);
	return ;
}

void MultiGcdZ(sparse_z & r, const sparse_z & f,const sparse_z & g)
{
	if(f.size()==0)
	{
		copy_sparse_z(r,g);
		return ;
	}
	if(g.size()==0)
	{
		copy_sparse_z(r,f);
		return ;
	}
	uint n=f[0].exponents.size();
	//MultiGcdZ_Dense_Interpolation(r,f,g,n); //145
	MultiGcdZ_Sparse(r,f,g,n); //135
	return ;
}

}
