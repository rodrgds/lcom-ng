#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdint.h>

#include "macros.h"

uint32_t sysInbCounter = 0;
uint8_t scannedKey;
uint8_t sizeBytes = 0;
uint8_t scanBytes[2];
int kbdHookID = KEYBOARD_IRQ;

int kbd_subscribe_int(uint8_t *bit_no)
{
  kbdHookID = (int)*bit_no;
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbdHookID) != OK)
  {
    return 1;
  }
  *bit_no = (uint8_t)BIT(*bit_no);
  return 0;
}

int kbd_unsubscribe_int()
{
  if (sys_irqrmpolicy(&kbdHookID) != OK)
  {
    return 1;
  }
  return 0;
}

void(kbc_ih)()
{
  uint8_t status, tempKey;
  util_sys_inb(KBC_STAT_REG, &status);
  if ((status & (PAR_ERR | TO_ERR)) == 0)
  {
    if (util_sys_inb(KBC_OUT_BUF, &tempKey) == 0)
    {
      scannedKey = tempKey;
    }
  }
}

void kbd_menu_input(int *option)
{
  scanBytes[sizeBytes] = scannedKey;
  sizeBytes++;
  int opt = *option;
  if (scannedKey != FIRST_BYTE_SCAN)
  {
    if ((scannedKey & BREAK_CODE) == 0)
    {
      if (scannedKey == ARR_UP || scannedKey == ARR_DOWN || scannedKey == ENTER)
      {
        if (scannedKey == ARR_DOWN)
        {
          opt++;
        }
        else if (scannedKey == ARR_UP)
        {
          opt--;
        }
      }
      if (opt < 1)
      {
        opt = 3;
      }
      else if (opt >= 4)
      {
        opt = 1;
      }
    }
    *option = opt;
    sizeBytes = 0;
  }
}

void kbd_game_input(int *input, int nPlanets)
{
  scanBytes[sizeBytes] = scannedKey;
  sizeBytes++;
  int ipt = *input;
  if (scannedKey != FIRST_BYTE_SCAN)
  {
    if ((scannedKey & BREAK_CODE) == 0)
    {
      if (scannedKey == ARR_LEFT || scannedKey == ARR_RIGHT || scannedKey == ENTER)
      {
        if (scannedKey == ARR_RIGHT)
        {
          ipt++;
        }
        else if (scannedKey == ARR_LEFT)
        {
          ipt--;
        }
      }
      if (ipt < 0)
      {
        ipt = nPlanets - 1;
      }
      else if (ipt >= nPlanets)
      {
        ipt = 0;
      }
      *input = ipt;
      sizeBytes = 0;
    }
  }
}
