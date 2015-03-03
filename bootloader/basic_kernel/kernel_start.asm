;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; This code was made for the tutorial:
;; "Making a Simple C kernel with Basic printf and clearscreen Functions"
;;
;; This code comes with absolutly
;; NO WARRANTY
;; you can not hold me(KJ), nor
;; anyone else responsible for what
;; this code does.
;;
;; This code is in the public domain,
;; you may use it however you want
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 32]

[global start]
[extern _k_main] ; this is in the c file

start:
  call _k_main

  cli  ; stop interrupts
  hlt ; halt the CPU
