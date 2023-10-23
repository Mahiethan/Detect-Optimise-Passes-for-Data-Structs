; ModuleID = 'static_AoS.c'
source_filename = "static_AoS.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.nodeOne = type { i32, i32, i8 }
%struct.nodeTwo = type { i32, i8, i8 }
%struct.nodeThree = type { i32, float, double }

@.str = private unnamed_addr constant [11 x i8] c"Index %d:\0A\00", align 1
@.str.1 = private unnamed_addr constant [9 x i8] c"a = %d, \00", align 1
@.str.2 = private unnamed_addr constant [9 x i8] c"b = %d, \00", align 1
@.str.3 = private unnamed_addr constant [9 x i8] c"c = %c \0A\00", align 1
@AoS_Start = dso_local global i32 0, align 4
@AoS_End = dso_local global i32 0, align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeOne(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  store i32 0, ptr %5, align 4
  br label %6

6:                                                ; preds = %26, %2
  %7 = load i32, ptr %5, align 4
  %8 = load i32, ptr %4, align 4
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %29

10:                                               ; preds = %6
  %11 = load ptr, ptr %3, align 8
  %12 = load i32, ptr %5, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds %struct.nodeOne, ptr %11, i64 %13
  %15 = getelementptr inbounds %struct.nodeOne, ptr %14, i32 0, i32 0
  store i32 1, ptr %15, align 4
  %16 = load ptr, ptr %3, align 8
  %17 = load i32, ptr %5, align 4
  %18 = sext i32 %17 to i64
  %19 = getelementptr inbounds %struct.nodeOne, ptr %16, i64 %18
  %20 = getelementptr inbounds %struct.nodeOne, ptr %19, i32 0, i32 1
  store i32 2, ptr %20, align 4
  %21 = load ptr, ptr %3, align 8
  %22 = load i32, ptr %5, align 4
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds %struct.nodeOne, ptr %21, i64 %23
  %25 = getelementptr inbounds %struct.nodeOne, ptr %24, i32 0, i32 2
  store i8 97, ptr %25, align 4
  br label %26

26:                                               ; preds = %10
  %27 = load i32, ptr %5, align 4
  %28 = add nsw i32 %27, 1
  store i32 %28, ptr %5, align 4
  br label %6, !llvm.loop !6

29:                                               ; preds = %6
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @printArray(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  store i32 0, ptr %5, align 4
  br label %6

6:                                                ; preds = %35, %2
  %7 = load i32, ptr %5, align 4
  %8 = load i32, ptr %4, align 4
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %38

10:                                               ; preds = %6
  %11 = load i32, ptr %5, align 4
  %12 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %11)
  %13 = load ptr, ptr %3, align 8
  %14 = load i32, ptr %5, align 4
  %15 = sext i32 %14 to i64
  %16 = getelementptr inbounds %struct.nodeOne, ptr %13, i64 %15
  %17 = getelementptr inbounds %struct.nodeOne, ptr %16, i32 0, i32 0
  %18 = load i32, ptr %17, align 4
  %19 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %18)
  %20 = load ptr, ptr %3, align 8
  %21 = load i32, ptr %5, align 4
  %22 = sext i32 %21 to i64
  %23 = getelementptr inbounds %struct.nodeOne, ptr %20, i64 %22
  %24 = getelementptr inbounds %struct.nodeOne, ptr %23, i32 0, i32 1
  %25 = load i32, ptr %24, align 4
  %26 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %25)
  %27 = load ptr, ptr %3, align 8
  %28 = load i32, ptr %5, align 4
  %29 = sext i32 %28 to i64
  %30 = getelementptr inbounds %struct.nodeOne, ptr %27, i64 %29
  %31 = getelementptr inbounds %struct.nodeOne, ptr %30, i32 0, i32 2
  %32 = load i8, ptr %31, align 4
  %33 = sext i8 %32 to i32
  %34 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %33)
  br label %35

35:                                               ; preds = %10
  %36 = load i32, ptr %5, align 4
  %37 = add nsw i32 %36, 1
  store i32 %37, ptr %5, align 4
  br label %6, !llvm.loop !8

38:                                               ; preds = %6
  ret void
}

declare i32 @printf(ptr noundef, ...) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  %4 = alloca i64, align 8
  %5 = alloca i64, align 8
  %6 = alloca i64, align 8
  store i32 0, ptr %1, align 4
  store i32 697000, ptr %2, align 4
  %7 = load i32, ptr @AoS_Start, align 4
  %8 = load i32, ptr %2, align 4
  %9 = zext i32 %8 to i64
  %10 = call ptr @llvm.stacksave.p0()
  store ptr %10, ptr %3, align 8
  %11 = alloca %struct.nodeOne, i64 %9, align 16
  store i64 %9, ptr %4, align 8
  %12 = load i32, ptr @AoS_End, align 4
  store i32 12432, ptr %2, align 4
  %13 = load i32, ptr @AoS_Start, align 4
  %14 = load i32, ptr %2, align 4
  %15 = zext i32 %14 to i64
  %16 = alloca %struct.nodeTwo, i64 %15, align 16
  store i64 %15, ptr %5, align 8
  %17 = load i32, ptr @AoS_End, align 4
  store i32 124, ptr %2, align 4
  %18 = load i32, ptr @AoS_Start, align 4
  %19 = load i32, ptr %2, align 4
  %20 = zext i32 %19 to i64
  %21 = alloca %struct.nodeThree, i64 %20, align 16
  store i64 %20, ptr %6, align 8
  %22 = load i32, ptr @AoS_End, align 4
  store i32 0, ptr %1, align 4
  %23 = load ptr, ptr %3, align 8
  call void @llvm.stackrestore.p0(ptr %23)
  %24 = load i32, ptr %1, align 4
  ret i32 %24
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare ptr @llvm.stacksave.p0() #2

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.stackrestore.p0(ptr) #2

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nocallback nofree nosync nounwind willreturn }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.0.0 (https://github.com/llvm/llvm-project.git 893416051d517d979481510fe3b52d83ba0d1e01)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
