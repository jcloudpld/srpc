Simple RPC(Remote Procedure Call)
=================================

1. 소개
  C++의 템플릿과 매크로를 이용하여 사용하기 쉽고 효율적이며 단순한 구조의
멀티플랫폼 원격 프로시져 호출(Remote Procedure Call)을 구현합니다.


2. 지원 환경
  * Win32
    - Visual C++ 7.1 - OK
    - Visual C++ 8 - OK
    - Cygwin (GCC 3.4.4), Mingw는 아직 테스트해 보지 못했습니다.
    - Visual C++ 6 - class template의 Partial template specialization이
      동작하지 않으므로 현재로선 지원하지 않습니다.
  * Linux
    - Ubuntu Breezy & Dapper(GCC 4.x) - OK
    - Debian (GCC >= 3.3.5) - OK
  * FreeBSD
    - FreeBSD 5.4 (GCC 3.4.2) - OK

3. 컴파일
  * see COMPILE.txt, DEPENDENCIES.txt
