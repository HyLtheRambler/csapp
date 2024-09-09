
a.out:     file format elf64-x86-64


Disassembly of section .init:

0000000000400418 <_init>:
  400418:	48 83 ec 08          	sub    $0x8,%rsp
  40041c:	48 8b 05 d5 0b 20 00 	mov    0x200bd5(%rip),%rax        # 600ff8 <__gmon_start__>
  400423:	48 85 c0             	test   %rax,%rax
  400426:	74 05                	je     40042d <_init+0x15>
  400428:	e8 43 00 00 00       	callq  400470 <__gmon_start__@plt>
  40042d:	48 83 c4 08          	add    $0x8,%rsp
  400431:	c3                   	retq   

Disassembly of section .plt:

0000000000400440 <.plt>:
  400440:	ff 35 c2 0b 20 00    	pushq  0x200bc2(%rip)        # 601008 <_GLOBAL_OFFSET_TABLE_+0x8>
  400446:	ff 25 c4 0b 20 00    	jmpq   *0x200bc4(%rip)        # 601010 <_GLOBAL_OFFSET_TABLE_+0x10>
  40044c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400450 <printf@plt>:
  400450:	ff 25 c2 0b 20 00    	jmpq   *0x200bc2(%rip)        # 601018 <printf@GLIBC_2.2.5>
  400456:	68 00 00 00 00       	pushq  $0x0
  40045b:	e9 e0 ff ff ff       	jmpq   400440 <.plt>

0000000000400460 <__libc_start_main@plt>:
  400460:	ff 25 ba 0b 20 00    	jmpq   *0x200bba(%rip)        # 601020 <__libc_start_main@GLIBC_2.2.5>
  400466:	68 01 00 00 00       	pushq  $0x1
  40046b:	e9 d0 ff ff ff       	jmpq   400440 <.plt>

0000000000400470 <__gmon_start__@plt>:
  400470:	ff 25 b2 0b 20 00    	jmpq   *0x200bb2(%rip)        # 601028 <__gmon_start__>
  400476:	68 02 00 00 00       	pushq  $0x2
  40047b:	e9 c0 ff ff ff       	jmpq   400440 <.plt>

0000000000400480 <exit@plt>:
  400480:	ff 25 aa 0b 20 00    	jmpq   *0x200baa(%rip)        # 601030 <exit@GLIBC_2.2.5>
  400486:	68 03 00 00 00       	pushq  $0x3
  40048b:	e9 b0 ff ff ff       	jmpq   400440 <.plt>

Disassembly of section .text:

0000000000400490 <main>:
  400490:	48 83 ec 08          	sub    $0x8,%rsp
  400494:	b8 01 00 00 00       	mov    $0x1,%eax
  400499:	b9 02 00 00 00       	mov    $0x2,%ecx
  40049e:	66 0f 6f 05 8a 02 00 	movdqa 0x28a(%rip),%xmm0        # 400730 <__dso_handle+0x18>
  4004a5:	00 
  4004a6:	8d 50 01             	lea    0x1(%rax),%edx
  4004a9:	85 c9                	test   %ecx,%ecx
  4004ab:	89 0d d3 0b 20 00    	mov    %ecx,0x200bd3(%rip)        # 601084 <dst+0x4>
  4004b1:	89 05 c9 0b 20 00    	mov    %eax,0x200bc9(%rip)        # 601080 <dst>
  4004b7:	66 0f 7f 05 a1 0b 20 	movdqa %xmm0,0x200ba1(%rip)        # 601060 <src>
  4004be:	00 
  4004bf:	8b 0d a3 0b 20 00    	mov    0x200ba3(%rip),%ecx        # 601068 <src+0x8>
  4004c5:	0f 4e d0             	cmovle %eax,%edx
  4004c8:	bf 20 07 40 00       	mov    $0x400720,%edi
  4004cd:	8d 42 01             	lea    0x1(%rdx),%eax
  4004d0:	85 c9                	test   %ecx,%ecx
  4004d2:	89 0d b0 0b 20 00    	mov    %ecx,0x200bb0(%rip)        # 601088 <dst+0x8>
  4004d8:	8b 0d 8e 0b 20 00    	mov    0x200b8e(%rip),%ecx        # 60106c <src+0xc>
  4004de:	0f 4e c2             	cmovle %edx,%eax
  4004e1:	66 0f 6f 05 57 02 00 	movdqa 0x257(%rip),%xmm0        # 400740 <__dso_handle+0x28>
  4004e8:	00 
  4004e9:	8d 50 01             	lea    0x1(%rax),%edx
  4004ec:	85 c9                	test   %ecx,%ecx
  4004ee:	89 0d 98 0b 20 00    	mov    %ecx,0x200b98(%rip)        # 60108c <dst+0xc>
  4004f4:	b9 05 00 00 00       	mov    $0x5,%ecx
  4004f9:	0f 4e d0             	cmovle %eax,%edx
  4004fc:	85 c9                	test   %ecx,%ecx
  4004fe:	89 0d 8c 0b 20 00    	mov    %ecx,0x200b8c(%rip)        # 601090 <dst+0x10>
  400504:	8d 42 01             	lea    0x1(%rdx),%eax
  400507:	b1 06                	mov    $0x6,%cl
  400509:	66 0f 7f 05 5f 0b 20 	movdqa %xmm0,0x200b5f(%rip)        # 601070 <src+0x10>
  400510:	00 
  400511:	0f 4e c2             	cmovle %edx,%eax
  400514:	89 0d 7a 0b 20 00    	mov    %ecx,0x200b7a(%rip)        # 601094 <dst+0x14>
  40051a:	85 c9                	test   %ecx,%ecx
  40051c:	8d 50 01             	lea    0x1(%rax),%edx
  40051f:	8b 0d 53 0b 20 00    	mov    0x200b53(%rip),%ecx        # 601078 <src+0x18>
  400525:	0f 4e d0             	cmovle %eax,%edx
  400528:	8d 42 01             	lea    0x1(%rdx),%eax
  40052b:	85 c9                	test   %ecx,%ecx
  40052d:	89 0d 65 0b 20 00    	mov    %ecx,0x200b65(%rip)        # 601098 <dst+0x18>
  400533:	0f 4e c2             	cmovle %edx,%eax
  400536:	8b 15 40 0b 20 00    	mov    0x200b40(%rip),%edx        # 60107c <src+0x1c>
  40053c:	8d 70 01             	lea    0x1(%rax),%esi
  40053f:	85 d2                	test   %edx,%edx
  400541:	89 15 55 0b 20 00    	mov    %edx,0x200b55(%rip)        # 60109c <dst+0x1c>
  400547:	0f 4e f0             	cmovle %eax,%esi
  40054a:	31 c0                	xor    %eax,%eax
  40054c:	e8 ff fe ff ff       	callq  400450 <printf@plt>
  400551:	31 ff                	xor    %edi,%edi
  400553:	e8 28 ff ff ff       	callq  400480 <exit@plt>

0000000000400558 <_start>:
  400558:	31 ed                	xor    %ebp,%ebp
  40055a:	49 89 d1             	mov    %rdx,%r9
  40055d:	5e                   	pop    %rsi
  40055e:	48 89 e2             	mov    %rsp,%rdx
  400561:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  400565:	50                   	push   %rax
  400566:	54                   	push   %rsp
  400567:	49 c7 c0 00 07 40 00 	mov    $0x400700,%r8
  40056e:	48 c7 c1 90 06 40 00 	mov    $0x400690,%rcx
  400575:	48 c7 c7 90 04 40 00 	mov    $0x400490,%rdi
  40057c:	e8 df fe ff ff       	callq  400460 <__libc_start_main@plt>
  400581:	f4                   	hlt    
  400582:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  400589:	00 00 00 
  40058c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400590 <deregister_tm_clones>:
  400590:	b8 47 10 60 00       	mov    $0x601047,%eax
  400595:	55                   	push   %rbp
  400596:	48 2d 40 10 60 00    	sub    $0x601040,%rax
  40059c:	48 83 f8 0e          	cmp    $0xe,%rax
  4005a0:	48 89 e5             	mov    %rsp,%rbp
  4005a3:	77 02                	ja     4005a7 <deregister_tm_clones+0x17>
  4005a5:	5d                   	pop    %rbp
  4005a6:	c3                   	retq   
  4005a7:	b8 00 00 00 00       	mov    $0x0,%eax
  4005ac:	48 85 c0             	test   %rax,%rax
  4005af:	74 f4                	je     4005a5 <deregister_tm_clones+0x15>
  4005b1:	5d                   	pop    %rbp
  4005b2:	bf 40 10 60 00       	mov    $0x601040,%edi
  4005b7:	ff e0                	jmpq   *%rax
  4005b9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000004005c0 <register_tm_clones>:
  4005c0:	b8 40 10 60 00       	mov    $0x601040,%eax
  4005c5:	55                   	push   %rbp
  4005c6:	48 2d 40 10 60 00    	sub    $0x601040,%rax
  4005cc:	48 c1 f8 03          	sar    $0x3,%rax
  4005d0:	48 89 e5             	mov    %rsp,%rbp
  4005d3:	48 89 c2             	mov    %rax,%rdx
  4005d6:	48 c1 ea 3f          	shr    $0x3f,%rdx
  4005da:	48 01 d0             	add    %rdx,%rax
  4005dd:	48 d1 f8             	sar    %rax
  4005e0:	75 02                	jne    4005e4 <register_tm_clones+0x24>
  4005e2:	5d                   	pop    %rbp
  4005e3:	c3                   	retq   
  4005e4:	ba 00 00 00 00       	mov    $0x0,%edx
  4005e9:	48 85 d2             	test   %rdx,%rdx
  4005ec:	74 f4                	je     4005e2 <register_tm_clones+0x22>
  4005ee:	5d                   	pop    %rbp
  4005ef:	48 89 c6             	mov    %rax,%rsi
  4005f2:	bf 40 10 60 00       	mov    $0x601040,%edi
  4005f7:	ff e2                	jmpq   *%rdx
  4005f9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000400600 <__do_global_dtors_aux>:
  400600:	80 3d 39 0a 20 00 00 	cmpb   $0x0,0x200a39(%rip)        # 601040 <__TMC_END__>
  400607:	75 11                	jne    40061a <__do_global_dtors_aux+0x1a>
  400609:	55                   	push   %rbp
  40060a:	48 89 e5             	mov    %rsp,%rbp
  40060d:	e8 7e ff ff ff       	callq  400590 <deregister_tm_clones>
  400612:	5d                   	pop    %rbp
  400613:	c6 05 26 0a 20 00 01 	movb   $0x1,0x200a26(%rip)        # 601040 <__TMC_END__>
  40061a:	f3 c3                	repz retq 
  40061c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400620 <frame_dummy>:
  400620:	48 83 3d f8 07 20 00 	cmpq   $0x0,0x2007f8(%rip)        # 600e20 <__JCR_END__>
  400627:	00 
  400628:	74 1e                	je     400648 <frame_dummy+0x28>
  40062a:	b8 00 00 00 00       	mov    $0x0,%eax
  40062f:	48 85 c0             	test   %rax,%rax
  400632:	74 14                	je     400648 <frame_dummy+0x28>
  400634:	55                   	push   %rbp
  400635:	bf 20 0e 60 00       	mov    $0x600e20,%edi
  40063a:	48 89 e5             	mov    %rsp,%rbp
  40063d:	ff d0                	callq  *%rax
  40063f:	5d                   	pop    %rbp
  400640:	e9 7b ff ff ff       	jmpq   4005c0 <register_tm_clones>
  400645:	0f 1f 00             	nopl   (%rax)
  400648:	e9 73 ff ff ff       	jmpq   4005c0 <register_tm_clones>
  40064d:	0f 1f 00             	nopl   (%rax)

0000000000400650 <ncopy>:
  400650:	31 c0                	xor    %eax,%eax
  400652:	85 d2                	test   %edx,%edx
  400654:	7e 29                	jle    40067f <ncopy+0x2f>
  400656:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  40065d:	00 00 00 
  400660:	48 83 c7 04          	add    $0x4,%rdi
  400664:	8b 4f fc             	mov    -0x4(%rdi),%ecx
  400667:	44 8d 40 01          	lea    0x1(%rax),%r8d
  40066b:	48 83 c6 04          	add    $0x4,%rsi
  40066f:	85 c9                	test   %ecx,%ecx
  400671:	89 4e fc             	mov    %ecx,-0x4(%rsi)
  400674:	41 0f 4f c0          	cmovg  %r8d,%eax
  400678:	83 ea 01             	sub    $0x1,%edx
  40067b:	75 e3                	jne    400660 <ncopy+0x10>
  40067d:	f3 c3                	repz retq 
  40067f:	f3 c3                	repz retq 
  400681:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  400688:	00 00 00 
  40068b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000400690 <__libc_csu_init>:
  400690:	41 57                	push   %r15
  400692:	41 89 ff             	mov    %edi,%r15d
  400695:	41 56                	push   %r14
  400697:	49 89 f6             	mov    %rsi,%r14
  40069a:	41 55                	push   %r13
  40069c:	49 89 d5             	mov    %rdx,%r13
  40069f:	41 54                	push   %r12
  4006a1:	4c 8d 25 68 07 20 00 	lea    0x200768(%rip),%r12        # 600e10 <__frame_dummy_init_array_entry>
  4006a8:	55                   	push   %rbp
  4006a9:	48 8d 2d 68 07 20 00 	lea    0x200768(%rip),%rbp        # 600e18 <__init_array_end>
  4006b0:	53                   	push   %rbx
  4006b1:	4c 29 e5             	sub    %r12,%rbp
  4006b4:	31 db                	xor    %ebx,%ebx
  4006b6:	48 c1 fd 03          	sar    $0x3,%rbp
  4006ba:	48 83 ec 08          	sub    $0x8,%rsp
  4006be:	e8 55 fd ff ff       	callq  400418 <_init>
  4006c3:	48 85 ed             	test   %rbp,%rbp
  4006c6:	74 1e                	je     4006e6 <__libc_csu_init+0x56>
  4006c8:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  4006cf:	00 
  4006d0:	4c 89 ea             	mov    %r13,%rdx
  4006d3:	4c 89 f6             	mov    %r14,%rsi
  4006d6:	44 89 ff             	mov    %r15d,%edi
  4006d9:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  4006dd:	48 83 c3 01          	add    $0x1,%rbx
  4006e1:	48 39 eb             	cmp    %rbp,%rbx
  4006e4:	75 ea                	jne    4006d0 <__libc_csu_init+0x40>
  4006e6:	48 83 c4 08          	add    $0x8,%rsp
  4006ea:	5b                   	pop    %rbx
  4006eb:	5d                   	pop    %rbp
  4006ec:	41 5c                	pop    %r12
  4006ee:	41 5d                	pop    %r13
  4006f0:	41 5e                	pop    %r14
  4006f2:	41 5f                	pop    %r15
  4006f4:	c3                   	retq   
  4006f5:	90                   	nop
  4006f6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  4006fd:	00 00 00 

0000000000400700 <__libc_csu_fini>:
  400700:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000400704 <_fini>:
  400704:	48 83 ec 08          	sub    $0x8,%rsp
  400708:	48 83 c4 08          	add    $0x8,%rsp
  40070c:	c3                   	retq   
