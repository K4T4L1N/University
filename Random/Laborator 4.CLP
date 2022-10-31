(deftemplate persoana
(slot nume)
(multislot copii)
)

(deffacts init
(persoana (nume Liviu)  (copii Vlad Maria Nelu))
(persoana (nume Vlad) (copii Oana Mircea))
(persoana (nume Maria) (copii Dan))
(persoana (nume Nelu) (copii Paul Radu Sorin))

)

(defrule verisor
()

)