
a.out:     file format elf64-x86-64


Disassembly of section .init:

00000000004003a8 <_init>:
  4003a8:	48 83 ec 08          	sub    $0x8,%rsp
  4003ac:	48 8b 05 45 0c 20 00 	mov    0x200c45(%rip),%rax        # 600ff8 <__gmon_start__>
  4003b3:	48 85 c0             	test   %rax,%rax
  4003b6:	74 05                	je     4003bd <_init+0x15>
  4003b8:	e8 33 00 00 00       	callq  4003f0 <__gmon_start__@plt>
  4003bd:	48 83 c4 08          	add    $0x8,%rsp
  4003c1:	c3                   	retq   

Disassembly of section .plt:

00000000004003d0 <.plt>:
  4003d0:	ff 35 32 0c 20 00    	pushq  0x200c32(%rip)        # 601008 <_GLOBAL_OFFSET_TABLE_+0x8>
  4003d6:	ff 25 34 0c 20 00    	jmpq   *0x200c34(%rip)        # 601010 <_GLOBAL_OFFSET_TABLE_+0x10>
  4003dc:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004003e0 <__libc_start_main@plt>:
  4003e0:	ff 25 32 0c 20 00    	jmpq   *0x200c32(%rip)        # 601018 <__libc_start_main@GLIBC_2.2.5>
  4003e6:	68 00 00 00 00       	pushq  $0x0
  4003eb:	e9 e0 ff ff ff       	jmpq   4003d0 <.plt>

00000000004003f0 <__gmon_start__@plt>:
  4003f0:	ff 25 2a 0c 20 00    	jmpq   *0x200c2a(%rip)        # 601020 <__gmon_start__>
  4003f6:	68 01 00 00 00       	pushq  $0x1
  4003fb:	e9 d0 ff ff ff       	jmpq   4003d0 <.plt>

Disassembly of section .text:

0000000000400400 <_start>:
  400400:	31 ed                	xor    %ebp,%ebp
  400402:	49 89 d1             	mov    %rdx,%r9
  400405:	5e                   	pop    %rsi
  400406:	48 89 e2             	mov    %rsp,%rdx
  400409:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  40040d:	50                   	push   %rax
  40040e:	54                   	push   %rsp
  40040f:	49 c7 c0 c0 05 40 00 	mov    $0x4005c0,%r8
  400416:	48 c7 c1 50 05 40 00 	mov    $0x400550,%rcx
  40041d:	48 c7 c7 47 05 40 00 	mov    $0x400547,%rdi
  400424:	e8 b7 ff ff ff       	callq  4003e0 <__libc_start_main@plt>
  400429:	f4                   	hlt    
  40042a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000400430 <deregister_tm_clones>:
  400430:	b8 37 10 60 00       	mov    $0x601037,%eax
  400435:	55                   	push   %rbp
  400436:	48 2d 30 10 60 00    	sub    $0x601030,%rax
  40043c:	48 83 f8 0e          	cmp    $0xe,%rax
  400440:	48 89 e5             	mov    %rsp,%rbp
  400443:	77 02                	ja     400447 <deregister_tm_clones+0x17>
  400445:	5d                   	pop    %rbp
  400446:	c3                   	retq   
  400447:	b8 00 00 00 00       	mov    $0x0,%eax
  40044c:	48 85 c0             	test   %rax,%rax
  40044f:	74 f4                	je     400445 <deregister_tm_clones+0x15>
  400451:	5d                   	pop    %rbp
  400452:	bf 30 10 60 00       	mov    $0x601030,%edi
  400457:	ff e0                	jmpq   *%rax
  400459:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000400460 <register_tm_clones>:
  400460:	b8 30 10 60 00       	mov    $0x601030,%eax
  400465:	55                   	push   %rbp
  400466:	48 2d 30 10 60 00    	sub    $0x601030,%rax
  40046c:	48 c1 f8 03          	sar    $0x3,%rax
  400470:	48 89 e5             	mov    %rsp,%rbp
  400473:	48 89 c2             	mov    %rax,%rdx
  400476:	48 c1 ea 3f          	shr    $0x3f,%rdx
  40047a:	48 01 d0             	add    %rdx,%rax
  40047d:	48 d1 f8             	sar    %rax
  400480:	75 02                	jne    400484 <register_tm_clones+0x24>
  400482:	5d                   	pop    %rbp
  400483:	c3                   	retq   
  400484:	ba 00 00 00 00       	mov    $0x0,%edx
  400489:	48 85 d2             	test   %rdx,%rdx
  40048c:	74 f4                	je     400482 <register_tm_clones+0x22>
  40048e:	5d                   	pop    %rbp
  40048f:	48 89 c6             	mov    %rax,%rsi
  400492:	bf 30 10 60 00       	mov    $0x601030,%edi
  400497:	ff e2                	jmpq   *%rdx
  400499:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000004004a0 <__do_global_dtors_aux>:
  4004a0:	80 3d 85 0b 20 00 00 	cmpb   $0x0,0x200b85(%rip)        # 60102c <_edata>
  4004a7:	75 11                	jne    4004ba <__do_global_dtors_aux+0x1a>
  4004a9:	55                   	push   %rbp
  4004aa:	48 89 e5             	mov    %rsp,%rbp
  4004ad:	e8 7e ff ff ff       	callq  400430 <deregister_tm_clones>
  4004b2:	5d                   	pop    %rbp
  4004b3:	c6 05 72 0b 20 00 01 	movb   $0x1,0x200b72(%rip)        # 60102c <_edata>
  4004ba:	f3 c3                	repz retq 
  4004bc:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004004c0 <frame_dummy>:
  4004c0:	48 83 3d 58 09 20 00 	cmpq   $0x0,0x200958(%rip)        # 600e20 <__JCR_END__>
  4004c7:	00 
  4004c8:	74 1e                	je     4004e8 <frame_dummy+0x28>
  4004ca:	b8 00 00 00 00       	mov    $0x0,%eax
  4004cf:	48 85 c0             	test   %rax,%rax
  4004d2:	74 14                	je     4004e8 <frame_dummy+0x28>
  4004d4:	55                   	push   %rbp
  4004d5:	bf 20 0e 60 00       	mov    $0x600e20,%edi
  4004da:	48 89 e5             	mov    %rsp,%rbp
  4004dd:	ff d0                	callq  *%rax
  4004df:	5d                   	pop    %rbp
  4004e0:	e9 7b ff ff ff       	jmpq   400460 <register_tm_clones>
  4004e5:	0f 1f 00             	nopl   (%rax)
  4004e8:	e9 73 ff ff ff       	jmpq   400460 <register_tm_clones>
  4004ed:	0f 1f 00             	nopl   (%rax)

00000000004004f0 <sum_list>:
  4004f0:	b8 00 00 00 00       	mov    $0x0,%eax
  4004f5:	eb 07                	jmp    4004fe <sum_list+0xe>
  4004f7:	48 03 07             	add    (%rdi),%rax
  4004fa:	48 8b 7f 08          	mov    0x8(%rdi),%rdi
  4004fe:	48 85 ff             	test   %rdi,%rdi
  400501:	75 f4                	jne    4004f7 <sum_list+0x7>
  400503:	f3 c3                	repz retq 

0000000000400505 <rsum_list>:
  400505:	48 85 ff             	test   %rdi,%rdi
  400508:	74 12                	je     40051c <rsum_list+0x17>
  40050a:	53                   	push   %rbx
  40050b:	48 8b 1f             	mov    (%rdi),%rbx
  40050e:	48 8b 7f 08          	mov    0x8(%rdi),%rdi
  400512:	e8 ee ff ff ff       	callq  400505 <rsum_list>
  400517:	48 01 d8             	add    %rbx,%rax
  40051a:	eb 06                	jmp    400522 <rsum_list+0x1d>
  40051c:	b8 00 00 00 00       	mov    $0x0,%eax
  400521:	c3                   	retq   
  400522:	5b                   	pop    %rbx
  400523:	c3                   	retq   

0000000000400524 <copy_block>:
  400524:	b8 00 00 00 00       	mov    $0x0,%eax
  400529:	eb 15                	jmp    400540 <copy_block+0x1c>
  40052b:	48 8b 0f             	mov    (%rdi),%rcx
  40052e:	48 89 0e             	mov    %rcx,(%rsi)
  400531:	48 31 c8             	xor    %rcx,%rax
  400534:	48 83 ea 01          	sub    $0x1,%rdx
  400538:	48 8d 76 08          	lea    0x8(%rsi),%rsi
  40053c:	48 8d 7f 08          	lea    0x8(%rdi),%rdi
  400540:	48 85 d2             	test   %rdx,%rdx
  400543:	7f e6                	jg     40052b <copy_block+0x7>
  400545:	f3 c3                	repz retq 

0000000000400547 <main>:
  400547:	b8 01 00 00 00       	mov    $0x1,%eax
  40054c:	c3                   	retq   
  40054d:	0f 1f 00             	nopl   (%rax)

0000000000400550 <__libc_csu_init>:
  400550:	41 57                	push   %r15
  400552:	41 89 ff             	mov    %edi,%r15d
  400555:	41 56                	push   %r14
  400557:	49 89 f6             	mov    %rsi,%r14
  40055a:	41 55                	push   %r13
  40055c:	49 89 d5             	mov    %rdx,%r13
  40055f:	41 54                	push   %r12
  400561:	4c 8d 25 a8 08 20 00 	lea    0x2008a8(%rip),%r12        # 600e10 <__frame_dummy_init_array_entry>
  400568:	55                   	push   %rbp
  400569:	48 8d 2d a8 08 20 00 	lea    0x2008a8(%rip),%rbp        # 600e18 <__init_array_end>
  400570:	53                   	push   %rbx
  400571:	4c 29 e5             	sub    %r12,%rbp
  400574:	31 db                	xor    %ebx,%ebx
  400576:	48 c1 fd 03          	sar    $0x3,%rbp
  40057a:	48 83 ec 08          	sub    $0x8,%rsp
  40057e:	e8 25 fe ff ff       	callq  4003a8 <_init>
  400583:	48 85 ed             	test   %rbp,%rbp
  400586:	74 1e                	je     4005a6 <__libc_csu_init+0x56>
  400588:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  40058f:	00 
  400590:	4c 89 ea             	mov    %r13,%rdx
  400593:	4c 89 f6             	mov    %r14,%rsi
  400596:	44 89 ff             	mov    %r15d,%edi
  400599:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  40059d:	48 83 c3 01          	add    $0x1,%rbx
  4005a1:	48 39 eb             	cmp    %rbp,%rbx
  4005a4:	75 ea                	jne    400590 <__libc_csu_init+0x40>
  4005a6:	48 83 c4 08          	add    $0x8,%rsp
  4005aa:	5b                   	pop    %rbx
  4005ab:	5d                   	pop    %rbp
  4005ac:	41 5c                	pop    %r12
  4005ae:	41 5d                	pop    %r13
  4005b0:	41 5e                	pop    %r14
  4005b2:	41 5f                	pop    %r15
  4005b4:	c3                   	retq   
  4005b5:	90                   	nop
  4005b6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  4005bd:	00 00 00 

00000000004005c0 <__libc_csu_fini>:
  4005c0:	f3 c3                	repz retq 

Disassembly of section .fini:

00000000004005c4 <_fini>:
  4005c4:	48 83 ec 08          	sub    $0x8,%rsp
  4005c8:	48 83 c4 08          	add    $0x8,%rsp
  4005cc:	c3                   	retq   
