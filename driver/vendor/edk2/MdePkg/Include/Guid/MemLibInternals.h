#ifndef __MEM_LIB_INTERNALS__
#define __MEM_LIB_INTERNALS__
 
#include <Uefi.h>
 
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
 
VOID *
EFIAPI
InternalMemCopyMem (
  OUT     VOID        *Destination,
  IN      CONST VOID  *Source,
  IN      UINTN       Length
  );
 
VOID *
EFIAPI
InternalMemSetMem (
  OUT     VOID   *Buffer,
  IN      UINTN  Size,
  IN      UINT8  Value
  );
 
VOID *
EFIAPI
InternalMemSetMem16 (
  OUT     VOID    *Buffer,
  IN      UINTN   Length,
  IN      UINT16  Value
  );
 
VOID *
EFIAPI
InternalMemSetMem32 (
  OUT     VOID    *Buffer,
  IN      UINTN   Length,
  IN      UINT32  Value
  );
 
VOID *
EFIAPI
InternalMemSetMem64 (
  OUT     VOID    *Buffer,
  IN      UINTN   Length,
  IN      UINT64  Value
  );
 
VOID *
EFIAPI
InternalMemZeroMem (
  OUT     VOID   *Buffer,
  IN      UINTN  Length
  );
 
INTN
EFIAPI
InternalMemCompareMem (
  IN      CONST VOID  *DestinationBuffer,
  IN      CONST VOID  *SourceBuffer,
  IN      UINTN       Length
  );
 
CONST VOID *
EFIAPI
InternalMemScanMem8 (
  IN      CONST VOID  *Buffer,
  IN      UINTN       Length,
  IN      UINT8       Value
  );
 
CONST VOID *
EFIAPI
InternalMemScanMem16 (
  IN      CONST VOID  *Buffer,
  IN      UINTN       Length,
  IN      UINT16      Value
  );
 
CONST VOID *
EFIAPI
InternalMemScanMem32 (
  IN      CONST VOID  *Buffer,
  IN      UINTN       Length,
  IN      UINT32      Value
  );
 
CONST VOID *
EFIAPI
InternalMemScanMem64 (
  IN      CONST VOID  *Buffer,
  IN      UINTN       Length,
  IN      UINT64      Value
  );
 
BOOLEAN
EFIAPI
InternalMemIsZeroBuffer (
  IN CONST VOID  *Buffer,
  IN UINTN       Length
  );
 
#endif