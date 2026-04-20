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
- **Goal:** 여러 모듈이 객체의 생명주기를 공유해야 하는 서버 환경 이해.
- **Concepts:**
  - `std::shared_ptr`와 참조 카운팅(Reference Counting)의 내부 동작.
  - `use_count()`를 이용한 객체 소멸 타이밍 추적.
  - `weak_ptr`를 이용한 순환 참조(Circular Reference) 방지 기초.

---

## Tech Stack (Evolution)
| Category | Current (Finance) | Target (Game Server) |
| :--- | :--- | :--- |
| **Language** | C, Bash/Shell | **Modern C++ (20/23)** |
| **OS** | Unix, Linux | **Linux (Extreme Optimization)** |
| **Database** | MySQL, MariaDB | **MariaDB + Redis (Hybrid)** |
| **Network** | TCP/IP Basic | **Epoll-based Async I/O** |

---

