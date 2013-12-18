/*
 Copyright (c) 2013 OpenSourceRF.com.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "variant.h"

#include "Memory.h"

int ramUsed(void)
{
  return (uint32_t)&_end - (uint32_t)&_srelocate;
}

int flashUsed(void)
{
  return (uint32_t)&_etextrelocate - (uint32_t)&_sfixed;
}

int flashPageErase( uint8_t page )
{
  // prevent erase of reserved pages
  if (page < 124 || page > 251)
    return 1;

  // page contains sketch
  if (page <= PAGE_FROM_ADDRESS(&_etextrelocate))
    return 2;

  // enable flash page erase
  NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Een << NVMC_CONFIG_WEN_Pos);
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
    ;

  // erase page
  NRF_NVMC->ERASEPAGE = (page << 10);  // multiply by 1024
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
    ;

  // disable flash page erase
  NRF_NVMC->CONFIG &= ~(NVMC_CONFIG_WEN_Een << NVMC_CONFIG_WEN_Pos);
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
    ;

  // success
  return 0;
}

int flashWrite( uint32_t *address, uint32_t value )
{
  uint8_t page = PAGE_FROM_ADDRESS(address);

  // prevent erase of reserved pages
  if (page < 124 || page > 251)
    return 1;

  // page contains sketch
  if (page <= PAGE_FROM_ADDRESS(&_etextrelocate))
    return 2;

  // do not write to flash in the BLE radio is active
  while (RFduinoBLE_radioActive)
    ;

  // enable flash write
  NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos);
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
    ;

  // 22 to 43 us
  *address = value;

  // disable flash write
  NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
    ;

  // success
  return 0;
}

int flashWriteBlock( void *dst, const void *src, int cb )
{
  uint32_t *d = dst;
  const uint32_t *s = src;
  int rc;

  // convert count of bytes to count of uint32_t
  int n = (cb >> 2);

  // round up if remainder
  if (cb & 0x02)
    n++;

  // copy block one uint32_t at a time
  while (n-- > 0)
  {
    rc = flashWrite(d++, *s++);
    if (rc != 0)
      return rc;
  }

  return 0;
}
