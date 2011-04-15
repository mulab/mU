BeginPackage["Product`"]
Begin["`Private`"]
A[i_, {i_, n_}] := n!
A[i_, {i_, m_, n_}] := n! / (m - 1)!
A[i_^e_, {i_, n_}] := (n!)^e
A[i_^e_, {i_, m_, n_}] := (n! / (m - 1)!)^e
A[1 - (4 x_^2) / (Pi^2 (2k_-1)^2),{k_,1,Infinity}]:=Cos[x]
End[]
Prod := `Private`A;
EndPackage[]