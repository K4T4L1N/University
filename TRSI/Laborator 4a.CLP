(deftemplate persoana
(slot nume)
(multislot copii)
)

(deffacts init
(persoana (nume Liviu)  (copii Vlad Maria Nelu))
(persoana (nume Vlad) (copii Oana Mircea))
(persoana (nume Maria) (copii Dan ))
(persoana (nume Nelu) (copii Paul Radu Sorin ))

)

(defrule verisor
(persoana (nume ?b) (copii $? ?c $?) )
(persoana (nume ?p1) (copii $?c1) )
(persoana (nume ?p2) (copii $?c2 ) )
(persoana (nume ?p3) (copii $?c3 ) )
(test (neq ?c1 ?c2 ?c3))

=>
(printout t "Verii lui  " ?c3 " sunt " ?c2 ?c1   crlf)
)
