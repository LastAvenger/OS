/* int.c*/

#include "bootpack.h"
#include <stdio.h>
void init_pic(void)
/* PIC��ʼ��*/
{
	io_out8(PIC0_IMR,  0xff  ); //��ֹ��PIC�������ж�
	io_out8(PIC1_IMR,  0xff  ); //��ֹ��PIC�������ж�

    // ��PIC�趨
	io_out8(PIC0_ICW1, 0x11  ); //���ش���ģʽ (edge trigger mode)
	io_out8(PIC0_ICW2, 0x20  ); //IRQ0~7 �� INT 20~27 ����
	io_out8(PIC0_ICW3, 1 << 2); //PIC1��PIC��IRQ2 ����
	io_out8(PIC0_ICW4, 0x01  ); //�޻�����ģʽ

    //��PIC�趨
	io_out8(PIC1_ICW1, 0x11  ); //���ش���ģʽ (edge trigger mode)      
	io_out8(PIC1_ICW2, 0x28  ); // IRQ0~7 �� INT 28~2f ����
	io_out8(PIC1_ICW3, 2     ); //PIC1��IRQ2 ����
	io_out8(PIC1_ICW4, 0x01  ); //�޻�����ģʽ

	io_out8(PIC0_IMR,  0xfb  ); //11111011 PIC1 ����ȫ����ֹ
	io_out8(PIC1_IMR,  0xff  ); //11111111 ��ֹ�����ж� 

	return;
}

 
struct FIFO8 keyfifo;  //���̻���������
#define PORT_KEYDAT		0x0060

void inthandler21(int *esp)
/* ����PS/2���̵��ж�*/
{
	unsigned char data;
	io_out8(PIC0_OCW2, 0x61);	/* ֪ͨPIC, IRQ-01�������Ѿ����*/
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&keyfifo, data);
	return;
}


struct FIFO8 mousefifo;

void inthandler2c(int *esp)
/*����PS/2 �����ж� */
{
	unsigned char data;
	io_out8(PIC1_OCW2, 0x64);	/* ֪ͨPIC IRQ-12 �������Ѿ����*/
	io_out8(PIC0_OCW2, 0x62);	/* ֪ͨPIC IRQ-02 �������Ѿ����*/
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&mousefifo, data);
	return;
}

// ������ֵĴ���
void inthandler27(int *esp)
{
	io_out8(PIC0_OCW2, 0x67); 
	return;
}
