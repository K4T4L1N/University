(
    deffunction power (?first ?second)
        (** ?first ?second)
)

(
    defrule prod
        (num1 ?first)
        (num2 ?second) 
        =>
        (bind ?result (power ?first ?second))
        (printout t " S= " ?result crlf)
)