# Vulp

![C++ version](https://img.shields.io/badge/C++-17/20-blue.svg?style=flat)
[![Build](https://img.shields.io/github/workflow/status/tasts-robots/vulp/CI)](https://github.com/tasts-robots/vulp/actions)
[![Coverage](https://coveralls.io/repos/github/tasts-robots/vulp/badge.svg?branch=main)](https://coveralls.io/github/tasts-robots/vulp?branch=main)
[![Documentation](https://img.shields.io/badge/docs-online-brightgreen?logo=read-the-docs&style=flat)](https://tasts-robots.org/doc/vulp/)
[![Example project](https://img.shields.io/badge/Example-upkie_locomotion-green)](https://github.com/tasts-robots/upkie_locomotion)

Real-time motion control for Python. 🚧 **Pre-release.**

Vulp is a small inter-process communication (IPC) protocol that allows Python code to control faster actuators and simulators. It is suitable for tasks that require [real-time](https://en.wiktionary.org/wiki/real-time#English) but *not* high-frequency performance. The main example of such a task is balancing: there is [theoretical and empirical evidence](https://arxiv.org/pdf/1907.01805.pdf) that bipeds and quadrupeds can balance themselves as leisurely as 5–15 Hz, despite being frequently implemented at 200–1000 Hz.

<img src="https://user-images.githubusercontent.com/1189580/170735874-39550a66-5792-44a5-98e8-898a004dec39.png" width=500 align="right">

In Vulp, a fast program, called a _spine_, talks to a slow program, called an _agent_, in a standard action-observation loop. (Currently the library supports C++ spines and Python agents, other languages are welcome.) Spine and agent run in separate processes and exchange ``action`` and ``observation`` dictionaries through shared memory. In its simplest form, ``action`` is a set of joint commands and ``observation`` reports joint observations, but Vulp provides a pipeline API to grow more complex spines with additional controllers (for higher-level actions) and observers (for richer observations). For example, a spine can run an inverse kinematics solver that reads its targets from ``action``, or include a ground contact estimator that writes to ``observation``.

### Try it out!

<!-- GIF: https://user-images.githubusercontent.com/1189580/170491850-dfbb4786-12ff-4fe8-8080-9413d68acfc1.gif -->
<!-- Issue: https://github.com/github/feedback/discussions/17256 -->
<img src="https://user-images.githubusercontent.com/1189580/170496331-e1293dd3-b50c-40ee-9c2e-f75f3096ebd8.png" height="100" align="right" />

```console
git clone https://github.com/tasts-robots/upkie_locomotion.git
cd upkie_locomotion
./tools/bazelisk run -c opt //agents/blue_balancer:bullet
```

Connect a USB controller to move the robot around. 🎮

There is no dependency to install on Linux thanks to [Bazel](https://bazel.build/), which builds all dependencies and runs the Python agent in one go. (This will take a while the first time.) The syntax is the same to deploy to the Raspberry Pi of the [real robot](https://www.youtube.com/shorts/8b36XcCgh7s).

## Features and non-features

All design decisions have their pros and cons. Take a look at the features and non-features below to decide if Vulp is a fit to your use case.

### Features

- Run the same Python code on simulated and real robots
- Interfaces with to the [mjbots pi3hat](https://mjbots.com/products/mjbots-pi3hat-r4-4b) and mjbots actuators
- Interfaces with to the [Bullet](http://bulletphysics.org/) simulator
- Observer pipeline to extend observations
- 🏗️ Controller pipeline to extend actions
- Soft real-time: spine-agent loop interactions are predictable and repeatble
- Unit tested, and not only with end-to-end tests

### Non-features

- Low frequency: Vulp is designed for tasks that run in the 1–400 Hz range (like balancing bipeds or quadrupeds)
- Soft, not hard real-time guarantee: the code is empirically reliable by a large margin, that's it
- Weakly-typed IPC: typing is used within agents and spines, but the interface between them is only checked at runtime

### Alternatives

If any of the non-features is a no-go to you, you may also want to check out these existing alternatives:

* [mc\_rtc](https://github.com/jrl-umi3218/mc_rtc/) - C++ real-time control framework from which Vulp inherited, among others, the idea of running the same code on simulated and real robots. The choice of a weakly-typed dictionary-based IPC was also inspired by mc\_rtc's data store. C++ controllers are bigger cathedrals to build but they can run at higher frequencies.
* [moteus](https://pypi.org/project/moteus/) - Python bindings for moteus brushless controllers also [run well up to 200 Hz](https://github.com/tasts-robots/vulp/blob/main/doc/loop_cycles.md#moteus-python-api), which makes them a simpler alternative if you don't need the simulation/real-robot switch.
* [robot\_interfaces](https://github.com/open-dynamic-robot-initiative/robot_interfaces) - Similar IPC between non-realtime Python and real-time C++ processes. The main difference lies in the use of Python bindings and action/observation types (more overhead, more safeguards) where Vulp goes structureless (faster changes, faster blunders). Also, robot\_interfaces enforces process synchronization with a [time-series API](https://people.tuebingen.mpg.de/mpi-is-software/robotfingers/docs/robot_interfaces/doc/timeseries.html) while in Vulp this is up to the agent (most agents act greedily on the latest observation).

## Getting started

Check out [``vulp_example``](https://github.com/tasts-robots/vulp_example) to get started with a minimal Python agent and Vulp spine.

The [``upkie_locomotion``](https://github.com/tasts-robots/upkie_locomotion) repository has more advanced patterns such as custom observers and multi-stage agents.

## Q and A

### Performance

> How can motion control be real-time in Python, with garbage collection and all?

Python agents talk with Vulp spines via the ``SpineInterface``, which can process both actions and observations in [about 0.7 ± 0.3 ms](doc/loop_cycles.md). This leaves plenty of room to implement other control components in a low-frequency loop. You may also be surprised at how Python performance has improved in recent years (most "tricks" that were popular ten years ago have been optimized away in CPython 3.8+). To consider one data point, here are the cycle periods measured in a complete Python agent for Upkie (the Pink balancer from [`upkie_locomotion`](https://github.com/tasts-robots/upkie_locomotion)) running on a Raspberry Pi 4 Model B (Quad core ARM Cortex-A72 @ 1.5GHz). It performs non-trivial tasks like balancing and whole-body inverse kinematics by quadratic programming:

<p align="center">
    <img src="https://user-images.githubusercontent.com/1189580/172820003-ade5aee1-fdca-41d7-958a-baf397a2caa3.png" width="800">
</p>

Note that the aforementioned 0.7 ± 0.3 ms processing time happens on the Python side, and is thus included in the 5.0 ms cycles represented by the orange curve. Meanwhile the spine is set to a reference frequency of 1.0 kHz and its corresponding cycle period was measured here at 1.0 ± 0.05 ms.

> I just started a simulation spine but it's surprisingly slow, how come?

Make sure you switch Bazel's [compilation mode](https://bazel.build/reference/command-line-reference#flag--compilation_mode) to "opt" when running both robot experiments and simulations. The compilation mode is "fastbuild" by default. Note that it is totally fine to compile agents in "fastbuild" during development while testing them on a spine compiled in "opt" that keeps running in the background.

> I have a Bullet simulation where the robot balances fine, but the agent repeatedly warns it "Skipped X clock cycles". What could be causing this?

This happens when your CPU is not powerful enough to run the simulator in real-time along with your agent and spine. You can call [`Spine::simulate`](https://tasts-robots.org/doc/vulp/classvulp_1_1spine_1_1Spine.html#a886ef5562b33f365d86e77465dd86204) with ``nb_substeps = 1`` instead of ``Spine::run``, which will result in the correct simulation time from the agent's point of view but make the simulation slower than real-time from your point of view.

> I'm running a pi3hat spine, why are my timings more erratic than the ones plotted above?

Make sure you configure CPU isolation and set the scaling governor to ``performance`` for real-time performance on a Raspberry Pi.

### Design choices

> Why use dictionaries rather than an [interface description language](https://en.wikipedia.org/wiki/Interface_description_language) like Protocol Buffers?

Interface description languages formally specify an API, which costs some overhead to write down and maintain, but can bring versioning, breaking change detection and other benefits, especially when the API is stable. Vulp, on the other hand, is designed for prototyping with rapidly-changing APIs: the spec is in the code. If an agent and spine communicate with incompatible/incomplete actions/observations, execution will immediately break, begging for developers to fix it.

> Why the name "Vulp"?

[Vulp](https://en.wiktionary.org/wiki/vulp#Noun_2) means "fox" in Romansh, a language spoken in the Swiss canton of the Grisons. Foxes are arguably quite reliable in their reaction times 🦊
