# 내 마당에 운석이 떨어졌다! — 운석을 파괴하고 평온한 마당을 되찾는 방치형 클리커 게임

▶︎ [GitHub 저장소](https://github.com/J0Min/Clicker) | ▶︎ [프로젝트 상세 보고서 보기](https://docs.google.com/document/d/1wzSlCMr69SFbh2wpD1I36SOYWRjuD1u4whqpvkfgCsg/edit?usp=sharing)

## ☄️ 한 줄 요약

**장르**: 방치형 시뮬레이터 (클리커 기반) / **제작 기간**: 2026-05-11 \~ 2026-05-22 (약 2주) / **사용 기술**: C++20, Unreal Engine 5.7.4 / **1인 개발**

## 🚀 핵심 기능

1. **정밀한 파편 생성 시스템**

   * `Sphere Trace By Channel`을 활용해 타격 위치를 정확히 계산하여 즉각적인 파편 액터와 나이아가라 이펙트를 스폰합니다.
2. **효율적인 자동화 및 업그레이드 루프**

   * `ListView`와 인터페이스 기반의 상점 UI를 통해 기하급수적인 성장의 재미를 제공합니다.
3. **스마트 드론 수집 시스템**

   * 매니저 액터가 레벨 내 파편을 주기적으로 탐색하여 드론 배열에 매칭합니다.

## 🛠 내가 직접 만든 부분

* **게임 기획 및 설계**: 전체 게임 루프 및 경제 밸런스 설계
* **클라이언트 프로그래밍**: C++20과 블루프린트를 활용한 핵심 로직 구현
* **UI/UX 제작**: 리스트 뷰 활용 상점 시스템 및 HUD 레이아웃 구축
* **레벨 디자인**: 집 앞마당 콘셉트의 거점형 레벨 세팅 및 포스트 아포칼립스 연출

## 💡 기술적 도전 및 해결

**문제: 드론이 타겟 파편을 찾지 못하고 엇나가는 현상**

* **원인**: 이동 중 타겟 소멸로 인한 Null 참조 및 루트-메쉬 간의 좌표 불일치.
* **해결**: Null 체크 루틴 강화 및 `Set World Location`의 Teleport 옵션 조정을 통해 이동 정확도 확보.

## ⚙️ 기술 스택

* **언어/엔진**: C++20, Unreal Engine 5.7.4
* **환경**: Visual Studio 2026, Git/GitHub

## 📦 빌드 및 실행 방법

1. 저장소 클론 후 `.uproject` 파일 우클릭 -> **Generate Visual Studio project files**.
2. 생성된 `.sln` 파일을 열어 빌드 후 언리얼 에디터에서 실행.

## 📑 보고서 및 문서

* [📄 프로젝트 상세 보고서 (Google Docs)](https://docs.google.com/document/d/1wzSlCMr69SFbh2wpD1I36SOYWRjuD1u4whqpvkfgCsg/edit?usp=sharing)

