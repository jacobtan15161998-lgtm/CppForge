# GDB 调试 C++ 代码常用命令指南

本文档记录使用 GDB（GNU Debugger）调试 C++ 项目（如 `cppCoreAndConcurrency`）的完整流程与常用命令，适用于 Linux / WSL 环境。

---

## 一、环境准备

在开始前，确保系统已安装编译器和调试工具：

```bash
sudo apt update
sudo apt install build-essential gdb cmake
```

| 命令 | 说明 |
|------|------|
| `sudo apt update` | 更新软件包索引 |
| `sudo apt install build-essential gdb cmake` | 安装 GCC/G++ 编译器、GDB 调试器、CMake 构建工具 |

---

## 二、编译并进入 GDB 调试

### 步骤 1：创建构建目录并进入

```bash
mkdir -p build && cd build
```

> `mkdir -p`：递归创建目录（如果已存在则不报错）

### 步骤 2：生成 Debug 构建配置

```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

> `-DCMAKE_BUILD_TYPE=Debug`：开启调试信息（等价于 `-g` 编译选项），这样 GDB 才能映射源码行号、变量名等。

### 步骤 3：编译项目

```bash
make
```

> 编译生成带调试符号的可执行文件。

### 步骤 4：启动 GDB

```bash
gdb ./cppCoreAndConcurrency
```

> 进入 GDB 交互界面，等待输入调试命令。

---

## 三、GDB 常用命令详解

### 1. 设置线程模式

```gdb
set non-stop off
```

- **含义**：关闭 `non-stop` 模式。
- **说明**：在 `non-stop` 模式下，当一个线程命中断点时，其他线程继续运行。设置为 `off` 后，任意一个线程命中断点，**所有线程都会暂停**，方便整体分析程序状态。
- **适用场景**：多线程调试时，通常建议关闭，便于观察全局状态。

---

```gdb
show non-stop
```

- **含义**：查看当前 `non-stop` 模式的设置值。
- **输出示例**：`non-stop mode is currently "off".`

---

### 2. 设置断点（Breakpoint）

#### 按函数名设置断点

```gdb
break main
b Worker::doWork
```

- `break` 与 `b` 等价，都是设置断点的命令。
- 程序运行到指定函数时会自动暂停。

#### 按文件名 + 行号设置断点

```gdb
break main.cpp:20
b Worker.cpp:45
```

- 程序运行到 `main.cpp` 第 20 行时暂停。
- 非常适合精确定位到某一行代码。

---

### 3. 查看线程信息

```gdb
info threads
```

- **含义**：列出当前进程中的所有线程。
- **输出字段**：
  - `Id`：线程编号
  - `Target Id`：系统级线程标识
  - `Frame`：该线程当前停在哪一行代码

---

### 4. 打印变量值

```gdb
print variableName
p x
p this
p *ptr
```

| 命令 | 说明 |
|------|------|
| `print x` / `p x` | 打印变量 `x` 的当前值 |
| `print *ptr` | 打印指针 `ptr` 指向的对象内容 |
| `print this` | 打印当前对象的地址（在类成员函数中） |

---

### 5. 控制程序执行

| 命令 | 缩写 | 说明 |
|------|------|------|
| `next` | `n` | **逐过程**执行，遇到函数调用不进入，直接执行完返回结果 |
| `step` | `s` | **逐语句**执行，遇到函数调用会进入函数内部 |
| `continue` | `c` | 从当前断点继续运行，直到下一个断点或程序结束 |
| `finish` | `fin` | 执行完当前函数，然后暂停在调用该函数的位置 |

> 💡 **记忆口诀**：`next` 把函数当一行看，`step` 钻进函数里看。

---

### 6. 查看调用栈（Backtrace）

```gdb
bt
backtrace
```

- **含义**：打印当前的函数调用栈。
- **用途**：当程序崩溃或命中断点时，快速定位**是谁调用了谁**，一路回溯到 `main`。
- **输出示例**：

```
#0  Worker::doWork (this=0x7fffffffe000) at Worker.cpp:20
#1  main () at main.cpp:15
```

---

### 7. 查看所有断点

```gdb
info breakpoints
```

- **含义**：列出所有已设置的断点及其信息。
- **输出字段**：
  - `Num`：断点编号
  - `Type`：断点类型
  - `Disp`：触发后是否保留
  - `Address`：断点地址
  - `What`：断点位置

---

### 8. 删除断点

```gdb
delete 1
```

- 删除编号为 `1` 的断点。

```gdb
delete
```

- **不带编号**时，GDB 会**删除所有断点**，需确认。

---

## 四、完整调试流程示例

```bash
# 1. 编译
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# 2. 启动 GDB
gdb ./cppCoreAndConcurrency

# 3. 在 GDB 中操作
(gdb) set non-stop off
(gdb) break main
(gdb) run
(gdb) break Worker.cpp:30
(gdb) continue
(gdb) info threads
(gdb) print counter
(gdb) bt
(gdb) next
(gdb) step
(gdb) finish
(gdb) quit
```

---

## 五、其他实用命令速查

| 命令 | 说明 |
|------|------|
| `run` / `r` | 启动程序运行 |
| `run arg1 arg2` | 带命令行参数启动程序 |
| `quit` / `q` | 退出 GDB |
| `list` / `l` | 查看当前位置附近的源代码 |
| `watch var` | 设置观察点，变量变化时暂停 |
| `thread apply all bt` | 打印所有线程的调用栈 |
| `thread 2` | 切换到编号为 2 的线程 |
| `set var x = 10` | 在调试时修改变量值 |
| `display x` | 每次暂停时自动打印变量 `x` |
| `undisplay` | 取消自动显示 |
| `help command` | 查看某个命令的详细帮助 |

---

> 📌 **小贴士**：GDB 命令支持 **Tab 补全**，输入前几个字母再按 Tab 即可自动补全命令名。
