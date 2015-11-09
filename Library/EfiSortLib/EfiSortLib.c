#include <Uefi.h>

#include <Library/DebugLib.h>
#include <Library/EfiSortLib.h>

// BubbleSort
/// 
/// @param
///
/// @return
/// @retval
VOID
BubbleSort (
  IN OUT UINT16 *Operand,
  IN     UINTN  NoChilds
  ) // sub_72C
{
  UINTN  NoRemainingChilds;
  UINTN  Index;
  UINTN  NoRemainingChilds2;
  UINT16 *OperandPtr;
  UINT16 FirstChild;

  ASSERT (Operand != NULL);
  ASSERT (NoChilds != 0);

  if (Operand > 0) {
    ++Operand;
    NoRemainingChilds = (NoChilds - 1);
    Index             = 1;

    do {
      NoRemainingChilds2 = NoRemainingChilds;
      OperandPtr         = Operand;

      if (Index < NoChilds) {
        do {
          FirstChild = Operand[-1];

          if (FirstChild > *OperandPtr) {
            Operand[-1] = *OperandPtr;
            *OperandPtr = FirstChild;
          }

          ++OperandPtr;
          --NoRemainingChilds2;
        } while (NoRemainingChilds2 > 0);
      }

      ++Index;
      ++Operand;
    } while ((NoRemainingChilds--) > 0);
  }
}
