#ifndef		__ARCH_ARM_V7_H__
#define		__ARCH_ARM_V7_H__

 
#ifdef	__cplusplus
extern "C"{
#endif

#include "def.h"

/*=========================================================================================*/
// For Cortex-A5,Cortex-A7,Cortex-A8,Cortex-A9,Cortex=A15

/*=========================================================================================*/

////CPSR
#define	USERMODE	    0x10
#define	FIQMODE			0x11
#define	IRQMODE		    0x12
#define	SVCMODE		    0x13
#define	ABORTMODE	    0x17
#define	UNDEFMODE	    0x1B
#define	SYSMODE		    0x1F
#define	MODEMASK	    0x1F
#define	NOINT		    0xC0

/*
 * CR1 bits (CP#15 CR1)
 */
#define R1_M    (1 << 0)        /* MMU enable                           */
#define R1_A    (1 << 1)        /* Alignment abort enable               */
#define R1_C    (1 << 2)        /* Dcache enable                        */
#define R1_W    (1 << 3)        /* Write buffer enable                  */
#define R1_P    (1 << 4)        /* 32-bit exception handler             */
#define R1_D    (1 << 5)        /* 32-bit data address range            */
#define R1_L    (1 << 6)        /* Implementation defined               */
#define R1_B    (1 << 7)        /* Big endian                           */
#define R1_S    (1 << 8)        /* System MMU protection                */
#define R1_R    (1 << 9)        /* ROM MMU protection                   */
#define R1_F    (1 << 10)       /* Implementation defined               */
#define R1_Z    (1 << 11)       /* Branch prediction enable				*/
#define R1_I    (1 << 12)       /* Icache enable                        */
#define R1_V    (1 << 13)       /* Vectors relocated to 0xffff0000      */
#define R1_RR   (1 << 14)       /* Round Robin cache replacement        */
#define R1_L4   (1 << 15)       /* LDR pc can set T bit                 */
#define R1_DT   (1 << 16)
#define R1_IT   (1 << 18)
#define R1_ST   (1 << 19)
#define R1_FI   (1 << 21)       /* Fast interrupt (lower latency mode)  */
#define R1_U    (1 << 22)       /* Unaligned access operation           */
#define R1_XP   (1 << 23)       /* Extended page tables                 */
#define R1_VE   (1 << 24)       /* Vectored interrupts                  */
#define R1_EE   (1 << 25)       /* Exception (Big) Endian               */
#define R1_TRE  (1 << 28)       /* TEX remap enable                     */
#define R1_AFE  (1 << 29)       /* Access flag enable                   */
#define R1_TE   (1 << 30)       /* Thumb exception enable               */

/*=========================================================================================*/
#define PAGE_SIZE           (4 * KB)                                    /*  ҳ���С                    */
#define PAGE_OFFSET         (12)                                        /*  ҳ���Сƫ��                */

#define SECTION_SIZE        (1 * MB)                                    /*  �δ�С                      */
#define SECTION_OFFSET      (20)                                        /*  �δ�Сƫ��                  */
#define SECTION_NR          (4096)                                      /*  ����                        */

//#define PHY_MEM_SIZE        (64 * MB)                                   /*  �����ڴ��С                */
//#define PHY_MEM_BASE        (0x30000000)                                /*  �����ڴ��ַ                */

//#define KERN_MEM_SIZE       (8 * MB)                                    /*  �ں��ڴ��С                */
//#define KERN_MEM_BASE       (PHY_MEM_BASE)                              /*  �ں��ڴ��ַ                */

//#define INT_MEM_SIZE        (1 * MB)                                    /*  �ж��ڴ��С                */
//#define INT_MEM_BASE        (KERN_MEM_BASE + KERN_MEM_SIZE)             /*  �ж��ڴ��ַ                */

//#define VMM_MEM_SIZE        (PHY_MEM_SIZE - KERN_MEM_SIZE-INT_MEM_SIZE) /*  VMM �ڴ��С                */
//#define VMM_MEM_BASE        (INT_MEM_BASE + INT_MEM_SIZE)               /*  VMM �ڴ��ַ                */
#define VMM_FRAME_SIZE      (PAGE_SIZE)                                 /*  ҳ���С                    */
//#define VMM_FRAME_NR        (VMM_MEM_SIZE / VMM_FRAME_SIZE)             /*  ҳ����                      */

#define PAGE_TBL_SIZE       (1 * KB)                                    /*  ҳ���С                    */
//#define PAGE_TBL_NR         (1024)                                      /*  ҳ����                      */
                                                                        /*  ҳ�������ַ                */
//#define PAGE_TBL_BASE       (KERN_MEM_BASE + KERN_MEM_SIZE - PAGE_TBL_NR * PAGE_TBL_SIZE)

#define MMU_TBL_SIZE        (16 * KB)                                   /*  MMU ת�����С              */
//#define MMU_TBL_BASE        (PAGE_TBL_BASE - MMU_TBL_SIZE)              /*  MMU ת�����ַ              */

/*=========================================================================================*/
//Domain
#define DOMAIN_NO_ACCESS        0
#define DOMAIN_CHECK            1
#define DOMAIN_NO_CHECK         3

//AP 
#define AP_ROM                  0
#define AP_USER_NO              1
#define AP_USER_RO              2  //supervisor=RW, user=RO
#define AP_USER_RW              3  //supervisor=RW, user=RW


//Cache + Buffer
#define	NCNB	0
#define	NCB		1
#define	CNB		2
#define	CB		3

#define	APX		0

#define	TEX0	0
#define	TEX1	1

#define	XN		0

#define	NS		0

//MMU L1����
#define	MMU_L1_FAULT	(0)

#define MMU_L1_SEC(tex,ap, domain, cb) \
	    ( (0<<19)|(0<<18)|(APX<<15) | (tex<<12) | (ap << 10) | (domain << 5) | (XN << 4) | (cb << 2) | (2) )



#define	MMU_L1_PAGE(domain)	  ( (domain << 5) |(0 << 4) | (NS<<3)| (1) )

//MMU L2����
#define	MMU_L2_4KB(tex,ap,cb)	  ( (APX<<8) | (tex << 6) | (ap << 4) | (cb<<2) | (1<<1) | (0<<0) )

/*=========================================================================================*/
/*=========================================================================================*/

typedef	 struct
{
	u32 L1[4096]; //ARM һ��ҳ��ռ4096����Ŀ.
	//u2 L2[16*256];
}mtt_buf_t;

typedef	struct	__mtt_addr
{
	u32	*pa;
	u32	*va;
}mtt_addr_t;

/*=========================================================================================*/
/*=========================================================================================*/

void	mmu_flush_icache(void);
void	mmu_flush_dcache(void);
void	mmu_flush_itlb(void);
void	mmu_flush_dtlb(void);

void	mmu_set_mtt_base(u32 tt_base_pa);
u32		mmu_get_mtt_base(void);
void	mmu_set_domain(u32 domain);
void	mmu_flush_tlb_all(void);

void mmu_map_section(u32 *L1,u32 va,u32 pa,u32 count,u32 attr);
u32  mmu_map_section_as_page(u32 *L1,u16 L1_idx,u32 *L2_PA);
void mmu_map_small_page(u32 *L2,u8 L2_idx,u32 mem_pa);

/*=========================================================================================*/
/*=========================================================================================*/
/*=========================================================================================*/
 
#ifdef	__cplusplus
}
#endif

#endif
