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

### [Day 2] Shared Ownership & Multi-threading Foundation (Upcoming)
### [Day 2] Shared Ownership & Reference Counting (2026-04-21)
- **Focus:** 다중 스레드/다중 모듈 환경을 대비한 메모리 생명주기 공유 시스템 구축.
- **Key Learnings:**
  - `std::shared_ptr`를 활용한 객체 상태 공유 메커니즘 구현 (`Monster` 클래스 설계).
  - Scope(스코프) 진입 및 탈출에 따른 `use_count()`(참조 카운트) 증감 논리 직접 검증. 카운트가 0이 되는 순간에만 메모리가 해제됨을 확인하여, 멀티스레드 환경에서의 메모리 누수 방지 원리 체득.
  - **Code Refactoring (C ? Modern C++):** C언어의 수동 메모리 해제 습관 탈피. 소멸자(`~Destructor`) 내부의 무의미한 멤버 변수 초기화 로직을 제거하고, 객체 생성 시 선언부 초기화를 강제하여 서버의 불필요한 연산 낭비 차단.
- **Artifact:** `day2_shared_ptr_monster.cpp`

---

## Tech Stack (Evolution)
| Category | Current (Finance) | Target (Game Server) |
| :--- | :--- | :--- |
| **Language** | C, Bash/Shell | **Modern C++ (20/23)** |
| **OS** | Unix, Linux | **Linux (Extreme Optimization)** |
| **Database** | MySQL, MariaDB | **MariaDB + Redis (Hybrid)** |
| **Network** | TCP/IP Basic | **Epoll-based Async I/O** |

---

