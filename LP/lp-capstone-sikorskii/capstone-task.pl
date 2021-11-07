:- ['dataFIle.pl'].

% Реализовать предикат проверки/поиска тещи



mother_in_law(M, X) :- male(M), child(C, M), findall(Z, child(C, Z), [H,T]), findall(L, child(T, L), [U,X]).

?- mother_in_law('Сокет Шарпов', X).