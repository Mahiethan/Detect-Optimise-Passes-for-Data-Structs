; ModuleID = 'mix_AoS.c'
source_filename = "mix_AoS.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.nodeOne = type { i32, i32, i8 }
%struct.nodeThree = type { i32, float, double }
%struct.nodeTwo = type { i32, i8, i8 }

@AoS_Start = dso_local global i32 0, align 4
@AoS_End = dso_local global i32 0, align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i64, align 8
  %7 = alloca ptr, align 8
  %8 = alloca i64, align 8
  %9 = alloca i64, align 8
  store i32 0, ptr %1, align 4
  store i32 697000, ptr %2, align 4
  %10 = load i32, ptr @AoS_Start, align 4
  %11 = load i32, ptr %2, align 4
  %12 = sext i32 %11 to i64
  %13 = mul i64 %12, 12
  %14 = call noalias ptr @malloc(i64 noundef %13) #4
  store ptr %14, ptr %3, align 8
  %15 = load i32, ptr @AoS_End, align 4
  store i32 7080, ptr %2, align 4
  %16 = load i32, ptr @AoS_Start, align 4
  %17 = load i32, ptr %2, align 4
  %18 = sext i32 %17 to i64
  %19 = mul i64 %18, 8
  %20 = call noalias ptr @malloc(i64 noundef %19) #4
  store ptr %20, ptr %4, align 8
  %21 = load i32, ptr @AoS_End, align 4
  store i32 697000, ptr %2, align 4
  %22 = load i32, ptr @AoS_Start, align 4
  %23 = load i32, ptr %2, align 4
  %24 = zext i32 %23 to i64
  %25 = call ptr @llvm.stacksave.p0()
  store ptr %25, ptr %5, align 8
  %26 = alloca %struct.nodeOne, i64 %24, align 16
  store i64 %24, ptr %6, align 8
  %27 = load i32, ptr @AoS_End, align 4
  store i32 372, ptr %2, align 4
  %28 = load i32, ptr @AoS_Start, align 4
  %29 = load i32, ptr %2, align 4
  %30 = sext i32 %29 to i64
  %31 = mul i64 %30, 16
  %32 = call noalias ptr @malloc(i64 noundef %31) #4
  store ptr %32, ptr %7, align 8
  %33 = load i32, ptr @AoS_End, align 4
  store i32 124, ptr %2, align 4
  %34 = load i32, ptr @AoS_Start, align 4
  %35 = load i32, ptr %2, align 4
  %36 = zext i32 %35 to i64
  %37 = alloca %struct.nodeThree, i64 %36, align 16
  store i64 %36, ptr %8, align 8
  %38 = load i32, ptr @AoS_End, align 4
  store i32 12432, ptr %2, align 4
  %39 = load i32, ptr @AoS_Start, align 4
  %40 = load i32, ptr %2, align 4
  %41 = zext i32 %40 to i64
  %42 = alloca %struct.nodeTwo, i64 %41, align 16
  store i64 %41, ptr %9, align 8
  %43 = load i32, ptr @AoS_End, align 4
  %44 = load ptr, ptr %3, align 8
  call void @free(ptr noundef %44) #5
  %45 = load ptr, ptr %4, align 8
  call void @free(ptr noundef %45) #5
  %46 = load ptr, ptr %7, align 8
  call void @free(ptr noundef %46) #5
  store i32 0, ptr %1, align 4
  %47 = load ptr, ptr %5, align 8
  call void @llvm.stackrestore.p0(ptr %47)
  %48 = load i32, ptr %1, align 4
  ret i32 %48
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare ptr @llvm.stacksave.p0() #2

; Function Attrs: nounwind
declare void @free(ptr noundef) #3

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.stackrestore.p0(ptr) #2

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nocallback nofree nosync nounwind willreturn }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind allocsize(0) }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.0.0 (https://github.com/llvm/llvm-project.git 893416051d517d979481510fe3b52d83ba0d1e01)"}
