/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */
typedef
unsigned
size_t
;
typedef
int
ptrdiff_t
;
typedef
unsigned
wchar_t
;
typedef
signed
char
int8_t
;
typedef
short
int16_t
;
typedef
int
int32_t
;
typedef
long
long
int64_t
;
typedef
unsigned
char
uint8_t
;
typedef
unsigned
short
uint16_t
;
typedef
unsigned
int
uint32_t
;
typedef
unsigned
long
long
uint64_t
;
typedef
int8_t
int_fast8_t
;
typedef
int
int_fast16_t
;
typedef
int
int_fast32_t
;
typedef
int64_t
int_fast64_t
;
typedef
unsigned
char
uint_fast8_t
;
typedef
unsigned
int
uint_fast16_t
;
typedef
unsigned
int
uint_fast32_t
;
typedef
uint64_t
uint_fast64_t
;
typedef
long
intptr_t
;
typedef
unsigned
long
uintptr_t
;
typedef
int8_t
int_least8_t
;
typedef
int16_t
int_least16_t
;
typedef
int32_t
int_least32_t
;
typedef
int64_t
int_least64_t
;
typedef
uint8_t
uint_least8_t
;
typedef
uint16_t
uint_least16_t
;
typedef
uint32_t
uint_least32_t
;
typedef
uint64_t
uint_least64_t
;
typedef
long
long
intmax_t
;
typedef
unsigned
long
long
uintmax_t
;
typedef
uint32_t
seL4_Word
;
typedef
seL4_Word
seL4_CPtr
;
typedef
seL4_CPtr
seL4_ARM_Page
;
typedef
seL4_CPtr
seL4_ARM_PageTable
;
typedef
seL4_CPtr
seL4_ARM_PageDirectory
;
typedef
seL4_CPtr
seL4_ARM_ASIDControl
;
typedef
seL4_CPtr
seL4_ARM_ASIDPool
;
typedef
struct
{
/* frame registers */
seL4_Word
pc
,
sp
,
cpsr
,
r0
,
r1
,
r8
,
r9
,
r10
,
r11
,
r12
;
/* other integer registers */
seL4_Word
r2
,
r3
,
r4
,
r5
,
r6
,
r7
,
r14
;
}
seL4_UserContext
;
typedef
enum
{
seL4_ARM_PageCacheable
=
0x01
,
seL4_ARM_ParityEnabled
=
0x02
,
seL4_ARM_Default_VMAttributes
=
0x03
,
/* seL4_ARM_PageCacheable | seL4_ARM_ParityEnabled */
_enum_pad_seL4_ARM_VMAttributes
=
(
1U
<<
(
(
sizeof
(
int
)
*
8
)
-
1
)
)
,
}
seL4_ARM_VMAttributes
;
struct
seL4_MessageInfo
{
uint32_t
words
[
1
]
;
}
;
typedef
struct
seL4_MessageInfo
seL4_MessageInfo_t
;
static
inline
seL4_MessageInfo_t
__attribute__
(
(
__const__
)
)
seL4_MessageInfo_new
(
uint32_t
label
,
uint32_t
capsUnwrapped
,
uint32_t
extraCaps
,
uint32_t
length
)
{
seL4_MessageInfo_t
seL4_MessageInfo
;
seL4_MessageInfo
.
words
[
0
]
=
0
;
seL4_MessageInfo
.
words
[
0
]
|=
(
label
&
0xfffff
)
<<
12
;
seL4_MessageInfo
.
words
[
0
]
|=
(
capsUnwrapped
&
0x7
)
<<
9
;
seL4_MessageInfo
.
words
[
0
]
|=
(
extraCaps
&
0x3
)
<<
7
;
seL4_MessageInfo
.
words
[
0
]
|=
(
length
&
0x7f
)
<<
0
;
return
seL4_MessageInfo
;
}
struct
seL4_CapData
{
uint32_t
words
[
1
]
;
}
;
typedef
struct
seL4_CapData
seL4_CapData_t
;
enum
seL4_CapData_tag
{
seL4_CapData_Badge
=
0
,
seL4_CapData_Guard
=
1
}
;
typedef
enum
seL4_CapData_tag
seL4_CapData_tag_t
;
typedef
enum
{
seL4_SysCall
=
-
1
,
seL4_SysReplyWait
=
-
2
,
seL4_SysSend
=
-
3
,
seL4_SysNBSend
=
-
4
,
seL4_SysWait
=
-
5
,
seL4_SysReply
=
-
6
,
seL4_SysYield
=
-
7
,
seL4_SysPoll
=
-
8
,
seL4_SysDebugPutChar
=
-
9
,
seL4_SysDebugHalt
=
-
10
,
seL4_SysDebugCapIdentify
=
-
11
,
seL4_SysDebugSnapshot
=
-
12
,
_enum_pad_seL4_Syscall_ID
=
(
1U
<<
(
(
sizeof
(
int
)
*
8
)
-
1
)
)
}
seL4_Syscall_ID
;
typedef
enum
api_object
{
seL4_UntypedObject
,
seL4_TCBObject
,
seL4_EndpointObject
,
seL4_NotificationObject
,
seL4_CapTableObject
,
seL4_NonArchObjectTypeCount
,
}
seL4_ObjectType
;
typedef
uint32_t
api_object_t
;
typedef
enum
{
seL4_NoError
=
0
,
seL4_InvalidArgument
,
seL4_InvalidCapability
,
seL4_IllegalOperation
,
seL4_RangeError
,
seL4_AlignmentError
,
seL4_FailedLookup
,
seL4_TruncatedMessage
,
seL4_DeleteFirst
,
seL4_RevokeFirst
,
seL4_NotEnoughMemory
,
}
seL4_Error
;
enum
priorityConstants
{
seL4_InvalidPrio
=
-
1
,
seL4_MinPrio
=
0
,
seL4_MaxPrio
=
255
}
;
enum
seL4_MsgLimits
{
seL4_MsgLengthBits
=
7
,
seL4_MsgExtraCapBits
=
2
}
;
typedef
enum
{
seL4_NoFault
=
0
,
seL4_CapFault
,
seL4_VMFault
,
seL4_UnknownSyscall
,
seL4_UserException
,
_enum_pad_seL4_FaultType
=
(
1U
<<
(
(
sizeof
(
int
)
*
8
)
-
1
)
)
,
}
seL4_FaultType
;
typedef
enum
{
seL4_NoFailure
=
0
,
seL4_InvalidRoot
,
seL4_MissingCapability
,
seL4_DepthMismatch
,
seL4_GuardMismatch
,
_enum_pad_seL4_LookupFailureType
=
(
1U
<<
(
(
sizeof
(
int
)
*
8
)
-
1
)
)
,
}
seL4_LookupFailureType
;
typedef
enum
{
seL4_CanWrite
=
0x01
,
seL4_CanRead
=
0x02
,
seL4_CanGrant
=
0x04
,
seL4_AllRights
=
0x07
,
/* seL4_CanWrite | seL4_CanRead | seL4_CanGrant */
seL4_Transfer_Mint
=
0x100
,
_enum_pad_seL4_CapRights
=
(
1U
<<
(
(
sizeof
(
int
)
*
8
)
-
1
)
)
,
}
seL4_CapRights
;
typedef
struct
seL4_IPCBuffer_
{
seL4_MessageInfo_t
tag
;
seL4_Word
msg
[
120
]
;
seL4_Word
userData
;
seL4_Word
caps_or_badges
[
(
(
1ul
<<
(
seL4_MsgExtraCapBits
)
)
-
1
)
]
;
seL4_CPtr
receiveCNode
;
seL4_CPtr
receiveIndex
;
seL4_Word
receiveDepth
;
}
seL4_IPCBuffer
; typedef
seL4_CPtr
seL4_CNode
;
typedef
seL4_CPtr
seL4_IRQHandler
;
typedef
seL4_CPtr
seL4_IRQControl
;
typedef
seL4_CPtr
seL4_TCB
;
typedef
seL4_CPtr
seL4_Untyped
;
typedef
seL4_CPtr
seL4_DomainSet
;
typedef
enum
_object
{
seL4_ARM_SmallPageObject
=
seL4_NonArchObjectTypeCount
,
seL4_ARM_LargePageObject
,
seL4_ARM_SectionObject
,
seL4_ARM_SuperSectionObject
,
seL4_ARM_PageTableObject
,
seL4_ARM_PageDirectoryObject
,
seL4_ObjectTypeCount
}
seL4_ArchObjectType
;
typedef
uint32_t
object_t
;
enum
invocation_label
{
InvalidInvocation
,
UntypedRetype
,
TCBReadRegisters
,
TCBWriteRegisters
,
TCBCopyRegisters
,
TCBConfigure
,
TCBSetPriority
,
TCBSetIPCBuffer
,
TCBSetSpace
,
TCBSuspend
,
TCBResume
,
TCBBindNotification
,
TCBUnbindNotification
,
CNodeRevoke
,
CNodeDelete
,
CNodeRecycle
,
CNodeCopy
,
CNodeMint
,
CNodeMove
,
CNodeMutate
,
CNodeRotate
,
CNodeSaveCaller
,
IRQIssueIRQHandler
,
IRQInterruptControl
,
IRQAckIRQ
,
IRQSetIRQHandler
,
IRQClearIRQHandler
,
DomainSetSet
,
nInvocationLabels
}
;
enum
arch_invocation_label
{
ARMPDClean_Data
=
nInvocationLabels
+
0
,
ARMPDInvalidate_Data
=
nInvocationLabels
+
1
,
ARMPDCleanInvalidate_Data
=
nInvocationLabels
+
2
,
ARMPDUnify_Instruction
=
nInvocationLabels
+
3
,
ARMPageTableMap
=
nInvocationLabels
+
4
,
ARMPageTableUnmap
=
nInvocationLabels
+
5
,
ARMPageMap
=
nInvocationLabels
+
6
,
ARMPageRemap
=
nInvocationLabels
+
7
,
ARMPageUnmap
=
nInvocationLabels
+
8
,
ARMPageClean_Data
=
nInvocationLabels
+
9
,
ARMPageInvalidate_Data
=
nInvocationLabels
+
10
,
ARMPageCleanInvalidate_Data
=
nInvocationLabels
+
11
,
ARMPageUnify_Instruction
=
nInvocationLabels
+
12
,
ARMPageGetAddress
=
nInvocationLabels
+
13
,
ARMASIDControlMakePool
=
nInvocationLabels
+
14
,
ARMASIDPoolAssign
=
nInvocationLabels
+
15
,
nArchInvocationLabels
}
;
enum
{
seL4_GlobalsFrame
=
0xffffc000
,
}
;
static
inline
void
seL4_Notify
(
seL4_CPtr
dest
,
seL4_Word
msg
)
{
register
seL4_Word
destptr
asm
(
"r0"
)
=
(
seL4_Word
)
dest
;
register
seL4_Word
info
asm
(
"r1"
)
=
seL4_MessageInfo_new
(
0
,
0
,
0
,
1
)
.
words
[
0
]
;
register
seL4_Word
msg0
asm
(
"r2"
)
=
msg
;
/* Perform the system call. */
register
seL4_Word
scno
asm
(
"r7"
)
=
seL4_SysSend
;
asm
volatile
(
"swi %[swi_num]"
:
"+r"
(
destptr
)
,
"+r"
(
msg0
)
,
"+r"
(
info
)
:
[
swi_num
]
"i"
(
(
seL4_SysSend
)
&
0x00ffffff
)
,
"r"
(
scno
)
:
"memory"
)
;
}
typedef
unsigned
long
__type_uint8_t_size_incorrect
[
(
sizeof
(
uint8_t
)
==
1
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_uint16_t_size_incorrect
[
(
sizeof
(
uint16_t
)
==
2
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_uint32_t_size_incorrect
[
(
sizeof
(
uint32_t
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_uint64_t_size_incorrect
[
(
sizeof
(
uint64_t
)
==
8
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_int_size_incorrect
[
(
sizeof
(
int
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_bool_size_incorrect
[
(
sizeof
(
_Bool
)
==
1
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_Word_size_incorrect
[
(
sizeof
(
seL4_Word
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_CapRights_size_incorrect
[
(
sizeof
(
seL4_CapRights
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_CapData_t_size_incorrect
[
(
sizeof
(
seL4_CapData_t
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_CPtr_size_incorrect
[
(
sizeof
(
seL4_CPtr
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_CNode_size_incorrect
[
(
sizeof
(
seL4_CNode
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_IRQHandler_size_incorrect
[
(
sizeof
(
seL4_IRQHandler
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_IRQControl_size_incorrect
[
(
sizeof
(
seL4_IRQControl
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_TCB_size_incorrect
[
(
sizeof
(
seL4_TCB
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_Untyped_size_incorrect
[
(
sizeof
(
seL4_Untyped
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_DomainSet_size_incorrect
[
(
sizeof
(
seL4_DomainSet
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_ARM_VMAttributes_size_incorrect
[
(
sizeof
(
seL4_ARM_VMAttributes
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_ARM_Page_size_incorrect
[
(
sizeof
(
seL4_ARM_Page
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_ARM_PageTable_size_incorrect
[
(
sizeof
(
seL4_ARM_PageTable
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_ARM_PageDirectory_size_incorrect
[
(
sizeof
(
seL4_ARM_PageDirectory
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_ARM_ASIDControl_size_incorrect
[
(
sizeof
(
seL4_ARM_ASIDControl
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_ARM_ASIDPool_size_incorrect
[
(
sizeof
(
seL4_ARM_ASIDPool
)
==
4
)
?
1
:
-
1
]
;
typedef
unsigned
long
__type_seL4_UserContext_size_incorrect
[
(
sizeof
(
seL4_UserContext
)
==
68
)
?
1
:
-
1
]
;
struct
seL4_ARM_Page_GetAddress
{
int
error
;
seL4_Word
paddr
;
}
;
typedef
struct
seL4_ARM_Page_GetAddress
seL4_ARM_Page_GetAddress_t
;
enum
{
seL4_CapNull
=
0
,
/* null cap */
seL4_CapInitThreadTCB
=
1
,
/* initial thread's TCB cap */
seL4_CapInitThreadCNode
=
2
,
/* initial thread's root CNode cap */
seL4_CapInitThreadPD
=
3
,
/* initial thread's PD cap */
seL4_CapIRQControl
=
4
,
/* global IRQ controller cap */
seL4_CapASIDControl
=
5
,
/* global ASID controller cap */
seL4_CapInitThreadASIDPool
=
6
,
/* initial thread's ASID pool cap */
seL4_CapIOPort
=
7
,
/* global IO port cap (null cap if not supported) */
seL4_CapIOSpace
=
8
,
/* global IO space cap (null cap if no IOMMU support) */
seL4_CapBootInfoFrame
=
9
,
/* bootinfo frame cap */
seL4_CapInitThreadIPCBuffer
=
10
,
/* initial thread's IPC buffer frame cap */
seL4_CapDomain
=
11
/* global domain controller cap */
}
;
typedef
struct
{
seL4_Word
start
;
/* first CNode slot position OF region */
seL4_Word
end
;
/* first CNode slot position AFTER region */
}
seL4_SlotRegion
;
typedef
struct
{
seL4_Word
basePaddr
;
/* base physical address of device region */
seL4_Word
frameSizeBits
;
/* size (2^n bytes) of a device-region frame */
seL4_SlotRegion
frames
;
/* device-region frame caps */
}
seL4_DeviceRegion
;
typedef
struct
{
seL4_Word
nodeID
;
/* ID [0..numNodes-1] of the seL4 node (0 if uniprocessor) */
seL4_Word
numNodes
;
/* number of seL4 nodes (1 if uniprocessor) */
seL4_Word
numIOPTLevels
;
/* number of IOMMU PT levels (0 if no IOMMU support) */
seL4_IPCBuffer
*
ipcBuffer
;
/* pointer to initial thread's IPC buffer */
seL4_SlotRegion
empty
;
/* empty slots (null caps) */
seL4_SlotRegion
sharedFrames
;
/* shared-frame caps (shared between seL4 nodes) */
seL4_SlotRegion
userImageFrames
;
/* userland-image frame caps */
seL4_SlotRegion
userImagePTs
;
/* userland-image PT caps */
seL4_SlotRegion
untyped
;
/* untyped-object caps (untyped caps) */
seL4_Word
untypedPaddrList
[
167
]
;
/* physical address of each untyped cap */
uint8_t
untypedSizeBitsList
[
167
]
;
/* size (2^n) bytes of each untyped cap */
uint8_t
initThreadCNodeSizeBits
;
/* initial thread's root CNode size (2^n slots) */
seL4_Word
numDeviceRegions
;
/* number of device regions */
seL4_DeviceRegion
deviceRegions
[
199
]
;
/* device regions */
uint8_t
initThreadDomain
;
/* Initial thread's domain ID */
}
seL4_BootInfo
;
int
EventFrom__run
(
void
)
{
/* Nothing required. */
return
0
;
}
void
EventFrom_emit_underlying
(
void
)
{
seL4_Notify
(
6
,
/* ignored */
0
)
;
}
