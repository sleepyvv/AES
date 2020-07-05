#include <stdio.h>
#include <string.h>


#include "openssl/evp.h"
#include "openssl/x509.h"

//加密函数
int  Encrypt_File()
{
 unsigned char key[EVP_MAX_KEY_LENGTH]; //保存密钥的数组
 unsigned char iv[EVP_MAX_KEY_LENGTH]; //保存初始化向量的数组
 EVP_CIPHER_CTX ctx;   //EVP加密上下文环境
 unsigned char out[1024];  //保存密文的缓冲区
 int outl;
 unsigned char in[1024];   //保存原文的缓冲区
 int inl;
 int rv;
 int i;
 FILE *fpIn;
 FILE *fpOut;
 //打开待加密文件
 fpIn = fopen("1.pdf","rb");
 if(fpIn==NULL)
 {
  return -1;
 }
 //打开保存密文的文件
 fpOut = fopen("C","wb");
 if(fpOut==NULL)
 {
  fclose(fpIn);
  return -1;
 }
 //设置key和iv
 for(i=0;i<24;i++)
 {
  key[i]=i;
 }
 for(i=0;i<8;i++)
 {
  iv[i]=i;
 }

 //初始化ctx
 EVP_CIPHER_CTX_init(&ctx);
 //设置密码算法、key和iv
 rv = EVP_EncryptInit_ex(&ctx,EVP_des_ede3_cbc(),NULL,key,iv);
 if(rv != 1)
 {
  printf("Err\n");
  return -1;
 }
 //循环读取原文，加密后后保存到密文文件。
 for(;;)
 {
  inl = fread(in,1,1024,fpIn);
  if(inl <= 0)//读取原文结束
   break;
  rv = EVP_EncryptUpdate(&ctx,out,&outl,in,inl);//加密
  if(rv != 1)
  {
   fclose(fpIn);
   fclose(fpOut);
   EVP_CIPHER_CTX_cleanup(&ctx);
   return -1;
  }
  fwrite(out,1,outl,fpOut);//保存密文到文件
 }
 //加密结束
 rv = EVP_EncryptFinal_ex(&ctx,out,&outl);
 if(rv != 1)
 {
  fclose(fpIn);
  fclose(fpOut);
  EVP_CIPHER_CTX_cleanup(&ctx);
  return -1;
 }
 fwrite(out,1,outl,fpOut);  //保密密文到文件
 fclose(fpIn);
 fclose(fpOut);
 EVP_CIPHER_CTX_cleanup(&ctx); //清除EVP加密上下文环境
 printf("加密已完成\n");
 return 1;
}


//解密函数
int Decrypt_File()
{
 unsigned char key[EVP_MAX_KEY_LENGTH];  //保存密钥的数组
 unsigned char iv[EVP_MAX_KEY_LENGTH];  //保存初始化向量的数组
 EVP_CIPHER_CTX ctx;    //EVP加密上下文环境
 unsigned char out[1024+EVP_MAX_KEY_LENGTH]; //保存解密后明文的缓冲区数组
 int outl;
 unsigned char in[1024];    //保存密文数据的数组
 int inl;
 int rv;
 int i;
 FILE *fpIn;
 FILE *fpOut;

 //打开待解密的密文文件
 fpIn = fopen("C","rb");
 if(fpIn==NULL)
 {
  return -1;
 }
 //打开保存明文的文件
 fpOut = fopen("M_new","wb");
 if(fpOut==NULL)
 {
  fclose(fpIn);
  return -1;
 }
 //设置key和iv
 for(i=0;i<24;i++)
 {
  key[i]=i;
 }
 for(i=0;i<8;i++)
 {
  iv[i]=i;
 }

 //初始化ctx
 EVP_CIPHER_CTX_init(&ctx);
 //设置解密的算法、key和iv
 rv = EVP_DecryptInit_ex(&ctx,EVP_des_ede3_cbc(),NULL,key,iv);
 if(rv != 1)
 {
  EVP_CIPHER_CTX_cleanup(&ctx);
  return -1;
 }
 //循环读取原文，解密后后保存到明文文件。
 for(;;)
 {
  inl = fread(in,1,1024,fpIn);
  if(inl <= 0)
   break;
  rv = EVP_DecryptUpdate(&ctx,out,&outl,in,inl);//解密
  if(rv != 1)
  {
   fclose(fpIn);
   fclose(fpOut);
   EVP_CIPHER_CTX_cleanup(&ctx);
   return -1;
  }
  fwrite(out,1,outl,fpOut);//保存明文到文件
 }
 //解密结束
 rv = EVP_DecryptFinal_ex(&ctx,out,&outl);
 if(rv != 1)
 {
  fclose(fpIn);
  fclose(fpOut);
  EVP_CIPHER_CTX_cleanup(&ctx);
  return -1;
 }
 fwrite(out,1,outl,fpOut);//保存明文到文件
 fclose(fpIn);
 fclose(fpOut);
 EVP_CIPHER_CTX_cleanup(&ctx);//清除EVP加密上下文环境
 printf("解密已完成\n");
 return 1;
}

int main()
{
 int i;
 int j;
 OpenSSL_add_all_algorithms();

 printf("|-------请您选择的服务类型--------|\n");
 printf("|                                |\n");
 printf("|    1:加密  2:解密  3:退出    |\n");
 printf("|                                |\n");
 printf("|---------------------------------|\n");
 printf("请输入选择:");
 scanf("%d",&i);


 switch(i)
 {
  case 1: Encrypt_File(); break;
  case 2: Decrypt_File(); break;
  case 3: exit(0); break;
  default:
   printf("        输入错误,请重新输入");
 }

 return 0;
}