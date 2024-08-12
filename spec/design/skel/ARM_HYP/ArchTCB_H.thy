(*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *)

theory ArchTCB_H
imports TCBDecls_H
begin
context Arch begin global_naming ARM_HYP_H

#INCLUDE_HASKELL SEL4/Object/TCB/ARM.lhs RegisterSet= CONTEXT ARM_HYP_H

#INCLUDE_HASKELL SEL4/Object/TCB.lhs Arch= ONLY archThreadGet archThreadSet


end
end
