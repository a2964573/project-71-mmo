# Road to MMORPG Server Developer (71-Day Intensive)

본 저장소는 실시간 MMORPG 서버 개발자로 거듭나기 위한 71일간의 기술적 도전과 성장을 기록합니다.

## Project Vision
- **Period:** 2026.04.20 ~ 2026.06.30 (71 Days)
- **Target Stack:** Modern C++ (20/23), Linux (Ubuntu), Epoll, Redis, MariaDB
- **Core Goal:** 리눅스 기반 고성능 비동기 서버 엔진 및 MMORPG 월드 로직 구현

---

## Daily Log & Milestones

### [Day 1] Memory Management & Ownership (2026-04-20)
- **Focus:** C의 수동 메모리 관리(`malloc`/`free`)에서 Modern C++의 **RAII** 및 **Smart Pointers**로 체질 개선.
- **Key Learnings:**
  - `std::unique_ptr`를 통한 단일 소유권 원칙 확립.
  - `std::move`를 이용한 소유권 이전(Move Semantics) 메커니즘 이해.
  - **Troubleshooting:** 소유권 이전 후 기존 포인터 접근 시 발생하는 `Segmentation Fault`를 확인하며, C++의 런타임 안전 메커니즘을 역설적으로 체득.
- **Artifact:** `day1_unique_ptr_example.cpp`

### [Day 2] Shared Ownership & Reference Counting (2026-04-21)
- **Focus:** 다중 스레드/다중 모듈 환경을 대비한 메모리 생명주기 공유 시스템 구축.
- **Key Learnings:**
  - `std::shared_ptr`를 활용한 객체 상태 공유 메커니즘 구현 (`Monster` 클래스 설계).
  - Scope(스코프) 진입 및 탈출에 따른 `use_count()`(참조 카운트) 증감 논리 직접 검증. 카운트가 0이 되는 순간에만 메모리가 해제됨을 확인하여, 멀티스레드 환경에서의 메모리 누수 방지 원리 체득.
  - **Code Refactoring (C ? Modern C++):** C언어의 수동 메모리 해제 습관 탈피. 소멸자(`~Destructor`) 내부의 무의미한 멤버 변수 초기화 로직을 제거하고, 객체 생성 시 선언부 초기화를 강제하여 서버의 불필요한 연산 낭비 차단.
- **Artifact:** `day2_shared_ptr_monster.cpp`

### [Day 3] Circular Reference & std::weak_ptr (2026-04-23)
- **Focus:** `shared_ptr` 순환 참조에 의한 메모리 누수(Memory Leak) 진단 및 `weak_ptr`를 이용한 해결.
- **Key Learnings:**
  - **순환 참조(Circular Reference) 매커니즘:** 두 객체가 서로를 강하게 참조할 때 참조 카운트가 0이 되지 않아 발생하는 '좀비 메모리' 현상을 코드로 직접 재현 및 확인.
  - **std::weak_ptr의 전략적 활용:** 객체를 소유하지 않고 관찰만 함으로써 참조 카운트 증가를 억제하고 순환 고리를 끊어냄.
  - **안전한 객체 접근 (lock & expired):** `weak_ptr`를 `shared_ptr`로 일시적 승격(`lock()`)하여 멀티스레드 환경에서도 안전하게 객체 생존 여부를 검증하고 데이터에 접근하는 실무 패턴 숙지.
  - **Refactoring:** 소멸자 내 불필요한 초기화(C 스타일)를 완전히 제거하고, C++의 자동 메모리 관리 철학에 집중.
- **Artifact:** `day3_weak_ptr_circular_fix.cpp`

### [Day 4] Multi-threading & Synchronization (2026-04-24)
- **Focus:** 멀티스레드 환경에서의 데이터 경합(Data Race) 진단 및 `std::mutex`를 이용한 스레드 동기화(Synchronization).
- **Key Learnings:**
  - `std::thread`를 활용한 동시성(Concurrency) 로직 구현 (Lambda 및 멤버 함수 포인터 활용).
  - 공유 자원(`pos`)에 대한 병렬 접근 시 발생하는 Lost Update(데이터 유실) 현상을 코드로 직접 유발하여 원인 분석.
  - `std::lock_guard`의 RAII 패턴을 활용하여 임계 구역(Critical Section)을 스코프 단위로 안전하게 락킹(Locking)하고 데드락(Deadlock)을 예방하는 구조 설계.
- **Artifact:** `day4_mutex_data_race.cpp`

### [Day 5] Lock-Free Atomic Operations & Thread-Safe I/O (2026-04-25)
- **Focus:** `std::mutex`의 컨텍스트 스위칭(Context Switching) 비용을 제거하는 하드웨어 단의 동기화 및 I/O 경합 해결.
- **Key Learnings:**
  - `std::atomic<int>`를 활용한 Lock-Free 카운터 구현. OS 락(Lock) 없이 CPU 원자적 명령어를 통해 초고속 데이터 무결성 보장.
  - 멀티스레드 환경에서 콘솔 출력(`std::cout`) 시 발생하는 화면 데이터 경합(I/O Interleaving) 원인 분석.
  - C++20 표준인 `std::osyncstream`을 도입하여, 연산(Atomic)과 로깅(Sync I/O)의 동기화 책임을 완벽히 분리하는 실무 아키텍처 설계 완료.
- **Artifact:** `day5_atomic_and_syncstream.cpp`

### [Day 6] Condition Variable & Event-Driven Architecture (2026-04-26)
- **Focus:** `std::condition_variable`을 활용한 생산자-소비자(Producer-Consumer) 패턴 구현 및 Busy-waiting 타파.
- **Key Learnings:**
  - CPU 자원을 무의미하게 낭비하는 무한 루프(Busy-waiting) 대신, 스레드를 수면(Sleep) 상태로 대기시키고 작업이 생겼을 때만 깨우는(Wake-up) 이벤트 기반 작업 큐 기초 설계.
  - `notify_one()`과 `notify_all()`의 역할 분리, 그리고 가짜 기상(Spurious Wakeup)을 방어하기 위한 조건식(`cv.wait()`) 이중 검증 패턴 숙지.
  - 서버 프로세스 종료 시 워커 스레드들이 큐에 남은 잔여 작업을 모두 처리하고 안전하게 종료되도록 유도하는 Graceful Shutdown 검증 완료.
- **Artifact:** `day6_condition_variable.cpp`

### [Day 7] Deadlock Prevention & std::scoped_lock (2026-04-27)
- **Focus:** 멀티스레드 환경의 교착 상태(Deadlock) 및 라이브락(Livelock) 진단, RAII 기반의 다중 락 획득 최적화.
- **Key Learnings:**
  - 두 객체가 서로의 자원을 교차로 요구할 때 발생하는 '환형 대기(Circular Wait)' 현상을 코드로 직접 유발하여 로직의 멈춤 현상 확인.
  - `try_lock`과 `std::thread::id`를 활용한 커스텀 Mutex 방어 로직 설계로 동시성 디버깅 역량 강화.
  - C++17 표준인 `std::scoped_lock`을 도입하여, 메모리 주소 기반 정렬 알고리즘을 통해 데드락을 원천 차단하고 수동 `unlock()`에 의한 휴먼 에러(미정의 동작)를 방지하는 Modern C++ 스레드 세이프 아키텍처 완성.
- **Artifact:** `day7_scoped_lock_deadlock_fix.cpp`

### [Day 8] POSIX TCP Socket & Thread-per-Client Architecture (2026-04-28)
- **Focus:** 리눅스 TCP 블로킹 소켓의 생명주기 이해 및 멀티스레딩을 이용한 다중 클라이언트 동시 접속 처리.
- **Key Learnings:**
  - `socket()`, `bind()`, `listen()`, `accept()` 시스템 콜을 통한 TCP 에코 서버 기초 구축.
  - 문자열 길이(`strlen`) 의존을 탈피하고, `read()` 함수의 반환값 검증을 통한 상태 기반(Stateful) 영구 연결 및 안전한 종료(Graceful Close) 로직 구현.
  - `std::thread`와 `detach()`를 결합하여 1 클라이언트당 1 전담 스레드를 배정하는 아키텍처를 설계하고, 다중 접속 시의 Non-Blocking 통신 증명.
  - 1:1 스레드 모델이 대규모 트래픽(C10K)에서 유발하는 스택 메모리 고갈 및 컨텍스트 스위칭 비용을 인지하고, 이벤트 기반 아키텍처로의 전환 당위성 도출.
- **Artifact:** `day8_multithread_tcp_server.cpp`

---

## Tech Stack (Evolution)
| Category | Current (Finance) | Target (Game Server) |
| :--- | :--- | :--- |
| **Language** | C, Bash/Shell | **Modern C++ (20/23)** |
| **OS** | Unix, Linux | **Linux (Extreme Optimization)** |
| **Database** | MySQL, MariaDB | **MariaDB + Redis (Hybrid)** |
| **Network** | TCP/IP Basic | **Epoll-based Async I/O** |

---

