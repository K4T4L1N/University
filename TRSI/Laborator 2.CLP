(deftemplate carte
(multislot prenume_autor)
(slot nume_autor)
(multislot titlu)
)

(deffacts init
(carte (nume_autor Calinescu) (prenume_autor George) (titlu "Enigma Otiliei"))
(carte (nume_autor Eliade) (prenume_autor Mircea) (titlu "Romanul Adolescentului Miop"))
(carte (nume_autor Preda) (prenume_autor Marin) (titlu "Morometii"))
(carte (nume_autor Sadoveanu) (prenume_autor Mihail) (titlu "Baltagul"))
(carte (nume_autor Preda) (prenume_autor Marin) (titlu "Cel Mai Iubit Dintre Pamanteni"))
(carte (nume_autor Rebreanu) (prenume_autor Liviu) (titlu "Ion"))
(carte (nume_autor Eliade) (prenume_autor Mircea) (titlu "Maitreyi"))
(carte (nume_autor Sadoveanu) (prenume_autor Mihail) (titlu "Hanu Ancutei"))
(carte (nume_autor Eliade) (prenume_autor Mircea) (titlu "Domnisoara Cristina"))
(carte (nume_autor George) (prenume_autor Calinescu) (titlu "Cartea Nuntii"))

)

(defrule cartea1
(carte (nume_autor Eliade) (prenume_autor ?t) (titlu ?p))
=>
(printout t ?p " Eliade " ?t crlf)
)