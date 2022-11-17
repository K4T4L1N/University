
(deffunction latura (?a1 ?a2 ?b1 ?b2)

   (sqrt (+ (** (- ?a2 ?a1) 2) (** (- ?b2 ?b1) 2)))

)

(deffunction gipotenuza (?a ?b)

   (sqrt (+ (** ?a 2) (** ?b 2)))

)

(deffunction perimetr (?a ?b ?c)

   ;%(bind ?c (gipotenuza ?a ?b))  


   (+ ?a ?b ?c) )                      


(defrule perim_triunghi

=>

(printout t "coordonatele punct A=")

(bind ?a1 4)
(bind ?a2 3)

(printout t "coordonatele punct B=")

(bind ?b1 7)
(bind ?b2 8)

(printout t "coordonatele punct C=")

(bind ?c1 7)
(bind ?c2 4)

(bind ?a (latura ?a1 ?a2 ?b1 ?b2))
(bind ?b (latura ?a1 ?a2 ?c1 ?c2))
(bind ?c (latura ?b1 ?b2 ?c1 ?c2))
(bind ?per (perimetr ?a ?b ?c))
(printout t "Latura perimetru=" ?per crlf) 
)