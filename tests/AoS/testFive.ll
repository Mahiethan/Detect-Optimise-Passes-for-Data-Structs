; ModuleID = 'mix_staticAoS.c'
source_filename = "mix_staticAoS.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.nodeThree = type { i32, float, double }
%struct.nodeOne = type { i32, i32, i8 }
%struct.nodeTwo = type { i32, i8, i8 }

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  %4 = alloca i64, align 8
  %5 = alloca i64, align 8
  %6 = alloca [1 x %struct.nodeThree], align 16
  %7 = alloca [100 x %struct.nodeOne], align 16
  %8 = alloca i64, align 8
  %9 = alloca [2344 x %struct.nodeOne], align 16
  %10 = alloca %struct.nodeOne, align 4
  %11 = alloca %struct.nodeTwo, align 4
  %12 = alloca %struct.nodeTwo, align 4
  store i32 0, ptr %1, align 4
  store i32 697000, ptr %2, align 4
  %13 = load i32, ptr %2, align 4
  %14 = zext i32 %13 to i64
  %15 = call ptr @llvm.stacksave.p0()
  store ptr %15, ptr %3, align 8
  %16 = alloca %struct.nodeOne, i64 %14, align 16
  store i64 %14, ptr %4, align 8
  store i32 12432, ptr %2, align 4
  %17 = load i32, ptr %2, align 4
  %18 = zext i32 %17 to i64
  %19 = alloca %struct.nodeTwo, i64 %18, align 16
  store i64 %18, ptr %5, align 8
  store i32 124, ptr %2, align 4
  %20 = load i32, ptr %2, align 4
  %21 = zext i32 %20 to i64
  %22 = alloca %struct.nodeThree, i64 %21, align 16
  store i64 %21, ptr %8, align 8
  store i32 0, ptr %1, align 4
  %23 = load ptr, ptr %3, align 8
  call void @llvm.stackrestore.p0(ptr %23)
  %24 = load i32, ptr %1, align 4
  ret i32 %24
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare ptr @llvm.stacksave.p0() #1

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.stackrestore.p0(ptr) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.0.0 (https://github.com/llvm/llvm-project.git 893416051d517d979481510fe3b52d83ba0d1e01)"}
