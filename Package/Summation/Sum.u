BeginPackage["Summation`"]
Begin["`Private`"]
A[i_, {i_, n_}] := 1/2 n (1 + n)
A[i_, {i_, m_, n_}] := 1/2 n (1 + n) - 1/2 m (m - 1)
A[i_^2, {i_, n_}] := 1/6 n (1 + n) (1 + 2 n)
A[i_^2, {i_, m_, n_}] := 1/6 n (1 + n) (1 + 2 n) - 1/6 (-1 + m) m (-1 + 2 m)
A[1/k_^2,{k_,1,Infinity}]:=Pi^2/6
A[a_. q_^k_,{k_,0,Infinity}]:=a/(1 - q)
A[(a_ +k_ r_)q_^k_,{k_,0,Infinity}]:=(a - a*q + q*r)/(-1 + q)^2
A[(-1)^(k_+1)1/k_,{k_,1,Infinity}]:=Log[2]
A[(-1)^(k_+1)1/(2k_-1),{k_,1,Infinity}]:=Pi/4
A[1/((4k_-1)(4k_+1)),{k_,1,Infinity}]:=(4 - Pi)/8
A[(-1)^(k_+1)1/k_,{k_,1,Infinity}]:=Log[2]
A[(-1)^(k_+1)1/k_^2,{k_,1,Infinity}]:=Pi^2/12
A[1/(2k_-1)^2,{k_,1,Infinity}]:=Pi^2/8
A[(-1)^k_/(2k_+1)^2,{k_,0,Infinity}]:=Catalan
A[(-1)^(k_+1)/(2k_-1)^3,{k_,1,Infinity}]:=Pi^3/32
A[1/(2k_-1)^4,{k_,1,Infinity}]:=Pi^4/96
A[(-1)^(k_+1)/(2k_-1)^5,{k_,1,Infinity}]:=(5*Pi^5)/1536
A[(-1)^(k_+1) k_/(k_+1)^2,{k_,1,Infinity}]:=Pi^2/12 - Log[2]
A[1/(k_ (2k_+1)),{k_,1,Infinity}]:=2 - Log[4]
A[Gamma[k_+1/2]/(k_^2 Gamma[k_]),{k_,1,Infinity}]:=Sqrt[Pi]*Log[4]
A[1/(4k_^2-1),{k_,1,Infinity}]:=1/2
A[1/(4k_^2-1)^2,{k_,1,Infinity}]:=(-8 + Pi^2)/16
A[1/(4k_^2-1)^3,{k_,1,Infinity}]:=1/2 - (3*Pi^2)/64
A[1/(4k_^2-1)^4,{k_,1,Infinity}]:=(-384 + 30*Pi^2 + Pi^4)/768
A[1/(k_ (4k_^2-1)),{k_,1,Infinity}]:=-1 + Log[4]
A[1/(k_ (9k_^2-1)),{k_,1,Infinity}]:=(3*(-1 + Log[3]))/2
A[1/(k_ (36k_^2-1)),{k_,1,Infinity}]:=-3 + (3*Log[3])/2 + Log[4]
A[k_/(4k_^2-1)^2,{k_,1,Infinity}]:=1/8
A[1/(k_ (4k_^2-1)^2),{k_,1,Infinity}]:=3/2 - Log[4]
A[(12k_^2-1)/(k_ (4k_^2-1)^2),{k_,1,Infinity}]:=Log[4]
A[1/(k_ (2k_+1)^2),{k_,1,Infinity}]:=4 - Pi^2/4 - Log[4]
A[1/((2k_-1)(2k_+1)),{k_,1,Infinity}]:=1/2
End[]
Sum := `Private`A;
EndPackage[]