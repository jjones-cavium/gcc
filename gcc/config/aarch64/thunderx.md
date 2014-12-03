;; Cavium Thunder pipeline description
;; Copyright (C) 2011 Free Software Foundation, Inc.
;;
;; Written by Andrew Pinski  <apinski@cavium.com>

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.


;; Thunder is a dual-issue processor.


(define_automaton "thunderx_main, thunderx_mult, thunderx_divide")

(define_cpu_unit "thunderx_pipe0" "thunderx_main")
(define_cpu_unit "thunderx_pipe1" "thunderx_main")
(define_cpu_unit "thunderx_mult" "thunderx_mult")
(define_cpu_unit "thunderx_divide" "thunderx_divide")

;; (final_presence_set "thunderx_pipe1" "thunderx_pipe0")

(define_insn_reservation "thunderx_alu" 1
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "adc_imm,adc_reg,adr,alu_imm,alu_reg,extend,logic_imm,logic_reg,mov_imm,mov_reg"))
  "thunderx_pipe0 | thunderx_pipe1")

(define_insn_reservation "thunderx_alus" 1
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "alus_imm,alus_reg,logics_imm,logics_reg"))
  "thunderx_pipe0 | thunderx_pipe1")

(define_insn_reservation "thunderx_shift" 1
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "bfm,extend,shift_imm,shift_reg"))
  "thunderx_pipe0 | thunderx_pipe1")


;; Arthimentic instructions with an extra shift or extend is two cycles.
;; FIXME: This needs more attributes on aarch64 than what is currently there;
;;    this is conserative for now.
;; Except this is not correct as this is only for !(LSL && shift by 0/1/2/3)
;; Except this is not correct as this is only for !(zero extend)

(define_insn_reservation "thunderx_arith_shift" 2
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "alu_ext,alu_shift_imm,alu_shift_reg,alus_ext,logic_shift_imm,logic_shift_reg,logics_shift_imm,logics_shift_reg,alus_shift_imm"))
  "thunderx_pipe0 | thunderx_pipe1")

(define_insn_reservation "thunderx_csel" 2
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "csel"))
  "thunderx_pipe0 | thunderx_pipe1")

;; Multiply and mulitply accumulate and count leading zeros can only happen on pipe 1

(define_insn_reservation "thunderx_mul" 4
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "mul,muls,mla,mlas,clz,smull,umull,smlal,umlal"))
  "thunderx_pipe1 + thunderx_mult")

;; crcb,crch,crcw is 4 cycles and can only happen on pipe 1

(define_insn_reservation "thunderx_crc32" 4
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "crc")
       (not (eq_attr "mode" "DI")))
  "thunderx_pipe1 + thunderx_mult")

;; crcx is 5 cycles and only happen on pipe 1
(define_insn_reservation "thunderx_crc64" 5
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "crc")
       (eq_attr "mode" "DI"))
  "thunderx_pipe1 + thunderx_mult")

(define_insn_reservation "thunderx_div32" 22
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "udiv,sdiv")
       (eq_attr "mode" "SI"))
  "thunderx_pipe1 + thunderx_divide, thunderx_divide * 21")

(define_insn_reservation "thunderx_div64" 38
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "udiv,sdiv")
       (eq_attr "mode" "DI"))
  "thunderx_pipe1 + thunderx_divide, thunderx_divide * 34")

;; Stores take one cycle in pipe 0
(define_insn_reservation "thunderx_store" 1
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "store1"))
  "thunderx_pipe0")

;; Store pair are single issued
(define_insn_reservation "thunderx_storepair" 1
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "store2"))
  "thunderx_pipe0 + thunderx_pipe1")


;; loads (and load pairs) from L1 take 3 cycles in pipe 0
(define_insn_reservation "thunderx_load" 3
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "load1, load2"))
  "thunderx_pipe0")

(define_insn_reservation "thunderx_brj" 1
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "branch"))
  "thunderx_pipe1")

(define_insn_reservation "thunderx_call" 1
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "call"))
  "thunderx_pipe1")

(define_insn_reservation "thunderx_trap" 1
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "trap"))
  "thunderx_pipe1")
;; FPU

(define_insn_reservation "thunderx_fadd" 4
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "faddd,fadds"))
  "thunderx_pipe1")

(define_insn_reservation "thunderx_fconst" 1
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "fconsts,fconstd"))
  "thunderx_pipe1")

;; Moves between fp are 2 cycles including min/max/select/abs/neg
(define_insn_reservation "thunderx_fmov" 2
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "fmov,f_minmaxs,f_minmaxd,f_sels,f_seld,ffarithd,ffariths"))
  "thunderx_pipe1")

(define_insn_reservation "thunderx_fmovgpr" 2
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "f_mrc, f_mcr"))
  "thunderx_pipe1")

(define_insn_reservation "thunderx_fmul" 6
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "fmacs,fmacd,fmuls,fmuld"))
  "thunderx_pipe1")

(define_insn_reservation "thunderx_fdivs" 12
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "fdivs"))
  "thunderx_pipe1 + thunderx_divide, thunderx_divide*8")

(define_insn_reservation "thunderx_fdivd" 22
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "fdivd"))
  "thunderx_pipe1 + thunderx_divide, thunderx_divide*18")

(define_insn_reservation "thunderx_fsqrts" 17
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "fsqrts"))
  "thunderx_pipe1 + thunderx_divide, thunderx_divide*13")

(define_insn_reservation "thunderx_fsqrtd" 28
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "fsqrtd"))
  "thunderx_pipe1 + thunderx_divide, thunderx_divide*31")

;; The rounding conversion inside fp is 4 cycles
(define_insn_reservation "thunderx_frint" 4
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "f_rints,f_rintd"))
  "thunderx_pipe1")

;; Float to integer with a move from int to/from float is 6 cycles
(define_insn_reservation "thunderx_f_cvt" 6
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "f_cvt,f_cvtf2i,f_cvti2f"))
  "thunderx_pipe1")

;; FP/SIMD load/stores happen in pipe 0
;; 64bit Loads register/pairs are 4 cycles from L1
(define_insn_reservation "thunderx_64simd_fp_load" 4
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "f_loadd,f_loads,neon_load1_2reg"))
  "thunderx_pipe0")

;; 128bit load pair is singled issue and 4 cycles from L1
(define_insn_reservation "thunderx_128simd_pair_load" 4
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "neon_load1_2reg_q"))
  "thunderx_pipe0+thunderx_pipe1")

;; FP/SIMD Stores takes one cycle in pipe 0
(define_insn_reservation "thunderx_simd_fp_store" 1
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "f_stored,f_stores"))
  "thunderx_pipe0")

;; 64bit neon store pairs are single issue for one cycle
(define_insn_reservation "thunderx_64neon_storepair" 1
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "neon_store1_2reg"))
  "thunderx_pipe0 + thunderx_pipe1")

;; 128bit neon store pair are single issued for two cycles
(define_insn_reservation "thunderx_128neon_storepair" 2
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "neon_store1_2reg_q"))
  "(thunderx_pipe0 + thunderx_pipe1)*2")

;; Assume both pipes are needed for unknown and multiple-instruction
;; patterns.

(define_insn_reservation "thunderx_unknown" 1
  (and (eq_attr "tune" "thunderx")
       (eq_attr "type" "untyped,multiple"))
  "thunderx_pipe0 + thunderx_pipe1")


;; In Thunder, the alus that take one cycle are combined with the branches
;; and don't take up an extra issue slot.
;; FIXME: Not taking up an extra issue slot is not modeled in 4.7
;; Also this is called macro fusion
(define_bypass 0 "thunderx_alus" "thunderx_brj")



