; ModuleID = 'dynamic_AoS.c'
source_filename = "dynamic_AoS.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.nodeTwo = type { i32, i8, i8 }
%struct.nodeOne = type { i32 }
%struct.nodeThree = type { i32, float, double }

@__const.setNodeTwo.n = private unnamed_addr constant %struct.nodeTwo { i32 1, i8 97, i8 0 }, align 4
@globalOne = dso_local global ptr null, align 8
@globalTwo = dso_local global ptr null, align 8
@.str = private unnamed_addr constant [11 x i8] c"Index %d:\0A\00", align 1
@.str.1 = private unnamed_addr constant [8 x i8] c"a = %d \00", align 1
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

6:                                                ; preds = %16, %2
  %7 = load i32, ptr %5, align 4
  %8 = load i32, ptr %4, align 4
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %19

10:                                               ; preds = %6
  %11 = load ptr, ptr %3, align 8
  %12 = load i32, ptr %5, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds %struct.nodeOne, ptr %11, i64 %13
  %15 = getelementptr inbounds %struct.nodeOne, ptr %14, i32 0, i32 0
  store i32 1, ptr %15, align 4
  br label %16

16:                                               ; preds = %10
  %17 = load i32, ptr %5, align 4
  %18 = add nsw i32 %17, 1
  store i32 %18, ptr %5, align 4
  br label %6, !llvm.loop !6

19:                                               ; preds = %6
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeTwo(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  store i32 0, ptr %5, align 4
  br label %6

6:                                                ; preds = %21, %2
  %7 = load i32, ptr %5, align 4
  %8 = load i32, ptr %4, align 4
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %24

10:                                               ; preds = %6
  %11 = load ptr, ptr %3, align 8
  %12 = load i32, ptr %5, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds %struct.nodeTwo, ptr %11, i64 %13
  %15 = getelementptr inbounds %struct.nodeTwo, ptr %14, i32 0, i32 0
  store i32 1, ptr %15, align 4
  %16 = load ptr, ptr %3, align 8
  %17 = load i32, ptr %5, align 4
  %18 = sext i32 %17 to i64
  %19 = getelementptr inbounds %struct.nodeTwo, ptr %16, i64 %18
  %20 = getelementptr inbounds %struct.nodeTwo, ptr %19, i32 0, i32 1
  store i8 97, ptr %20, align 4
  br label %21

21:                                               ; preds = %10
  %22 = load i32, ptr %5, align 4
  %23 = add nsw i32 %22, 1
  store i32 %23, ptr %5, align 4
  br label %6, !llvm.loop !8

24:                                               ; preds = %6
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i64 @setNodeTwo() #0 {
  %1 = alloca %struct.nodeTwo, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr align 4 %1, ptr align 4 @__const.setNodeTwo.n, i64 8, i1 false)
  %2 = load i64, ptr %1, align 4
  ret i64 %2
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeThree(i32 noundef %0, ptr noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  store ptr %1, ptr %4, align 8
  store i32 0, ptr %5, align 4
  br label %6

6:                                                ; preds = %26, %2
  %7 = load i32, ptr %5, align 4
  %8 = load i32, ptr %3, align 4
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %29

10:                                               ; preds = %6
  %11 = load ptr, ptr %4, align 8
  %12 = load i32, ptr %5, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds %struct.nodeThree, ptr %11, i64 %13
  %15 = getelementptr inbounds %struct.nodeThree, ptr %14, i32 0, i32 0
  store i32 1, ptr %15, align 8
  %16 = load ptr, ptr %4, align 8
  %17 = load i32, ptr %5, align 4
  %18 = sext i32 %17 to i64
  %19 = getelementptr inbounds %struct.nodeThree, ptr %16, i64 %18
  %20 = getelementptr inbounds %struct.nodeThree, ptr %19, i32 0, i32 1
  store float 2.000000e+00, ptr %20, align 4
  %21 = load ptr, ptr %4, align 8
  %22 = load i32, ptr %5, align 4
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds %struct.nodeThree, ptr %21, i64 %23
  %25 = getelementptr inbounds %struct.nodeThree, ptr %24, i32 0, i32 2
  store double 4.500000e+00, ptr %25, align 8
  br label %26

26:                                               ; preds = %10
  %27 = load i32, ptr %5, align 4
  %28 = add nsw i32 %27, 1
  store i32 %28, ptr %5, align 4
  br label %6, !llvm.loop !9

29:                                               ; preds = %6
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateGlobalOne(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  store i32 0, ptr %3, align 4
  br label %4

4:                                                ; preds = %14, %1
  %5 = load i32, ptr %3, align 4
  %6 = load i32, ptr %2, align 4
  %7 = icmp slt i32 %5, %6
  br i1 %7, label %8, label %17

8:                                                ; preds = %4
  %9 = load ptr, ptr @globalOne, align 8
  %10 = load i32, ptr %3, align 4
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds %struct.nodeOne, ptr %9, i64 %11
  %13 = getelementptr inbounds %struct.nodeOne, ptr %12, i32 0, i32 0
  store i32 1, ptr %13, align 4
  br label %14

14:                                               ; preds = %8
  %15 = load i32, ptr %3, align 4
  %16 = add nsw i32 %15, 1
  store i32 %16, ptr %3, align 4
  br label %4, !llvm.loop !10

17:                                               ; preds = %4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateGlobalTwo() #0 {
  %1 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  br label %2

2:                                                ; preds = %21, %0
  %3 = load i32, ptr %1, align 4
  %4 = icmp slt i32 %3, 1293
  br i1 %4, label %5, label %24

5:                                                ; preds = %2
  %6 = load ptr, ptr @globalTwo, align 8
  %7 = load i32, ptr %1, align 4
  %8 = sext i32 %7 to i64
  %9 = getelementptr inbounds %struct.nodeTwo, ptr %6, i64 %8
  %10 = getelementptr inbounds %struct.nodeTwo, ptr %9, i32 0, i32 0
  store i32 1, ptr %10, align 4
  %11 = load ptr, ptr @globalTwo, align 8
  %12 = load i32, ptr %1, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds %struct.nodeTwo, ptr %11, i64 %13
  %15 = getelementptr inbounds %struct.nodeTwo, ptr %14, i32 0, i32 1
  store i8 98, ptr %15, align 4
  %16 = load ptr, ptr @globalTwo, align 8
  %17 = load i32, ptr %1, align 4
  %18 = sext i32 %17 to i64
  %19 = getelementptr inbounds %struct.nodeTwo, ptr %16, i64 %18
  %20 = getelementptr inbounds %struct.nodeTwo, ptr %19, i32 0, i32 2
  store i8 97, ptr %20, align 1
  br label %21

21:                                               ; preds = %5
  %22 = load i32, ptr %1, align 4
  %23 = add nsw i32 %22, 1
  store i32 %23, ptr %1, align 4
  br label %2, !llvm.loop !11

24:                                               ; preds = %2
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

6:                                                ; preds = %20, %2
  %7 = load i32, ptr %5, align 4
  %8 = load i32, ptr %4, align 4
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %23

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
  br label %20

20:                                               ; preds = %10
  %21 = load i32, ptr %5, align 4
  %22 = add nsw i32 %21, 1
  store i32 %22, ptr %5, align 4
  br label %6, !llvm.loop !12

23:                                               ; preds = %6
  ret void
}

declare i32 @printf(ptr noundef, ...) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca ptr, align 8
  store i32 0, ptr %1, align 4
  store i32 697000, ptr %2, align 4
  %8 = load i32, ptr @AoS_Start, align 4
  %9 = load i32, ptr %2, align 4
  %10 = sext i32 %9 to i64
  %11 = mul i64 %10, 4
  %12 = call noalias ptr @malloc(i64 noundef %11) #5
  store ptr %12, ptr %3, align 8
  %13 = load i32, ptr @AoS_End, align 4
  store i32 0, ptr %4, align 4
  br label %14

14:                                               ; preds = %24, %0
  %15 = load i32, ptr %4, align 4
  %16 = load i32, ptr %2, align 4
  %17 = icmp slt i32 %15, %16
  br i1 %17, label %18, label %27

18:                                               ; preds = %14
  %19 = load ptr, ptr %3, align 8
  %20 = load i32, ptr %4, align 4
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds %struct.nodeOne, ptr %19, i64 %21
  %23 = getelementptr inbounds %struct.nodeOne, ptr %22, i32 0, i32 0
  store i32 1, ptr %23, align 4
  br label %24

24:                                               ; preds = %18
  %25 = load i32, ptr %4, align 4
  %26 = add nsw i32 %25, 1
  store i32 %26, ptr %4, align 4
  br label %14, !llvm.loop !13

27:                                               ; preds = %14
  %28 = load i32, ptr %2, align 4
  %29 = sext i32 %28 to i64
  %30 = mul i64 %29, 4
  %31 = call noalias ptr @malloc(i64 noundef %30) #5
  store ptr %31, ptr %5, align 8
  %32 = load i32, ptr %2, align 4
  %33 = sext i32 %32 to i64
  %34 = mul i64 %33, 4
  %35 = call noalias ptr @malloc(i64 noundef %34) #5
  store ptr %35, ptr @globalOne, align 8
  %36 = load i32, ptr %2, align 4
  %37 = sext i32 %36 to i64
  %38 = mul i64 %37, 8
  %39 = call noalias ptr @malloc(i64 noundef %38) #5
  store ptr %39, ptr @globalTwo, align 8
  %40 = load i32, ptr %2, align 4
  call void @populateGlobalOne(i32 noundef %40)
  call void @populateGlobalTwo()
  store i32 7080, ptr %2, align 4
  %41 = load i32, ptr @AoS_Start, align 4
  %42 = load i32, ptr %2, align 4
  %43 = sext i32 %42 to i64
  %44 = mul i64 %43, 8
  %45 = call noalias ptr @malloc(i64 noundef %44) #5
  store ptr %45, ptr %6, align 8
  %46 = load i32, ptr @AoS_End, align 4
  %47 = load ptr, ptr %6, align 8
  %48 = load i32, ptr %2, align 4
  call void @populateNodeTwo(ptr noundef %47, i32 noundef %48)
  store i32 372, ptr %2, align 4
  %49 = load i32, ptr @AoS_Start, align 4
  %50 = load i32, ptr %2, align 4
  %51 = sext i32 %50 to i64
  %52 = mul i64 %51, 16
  %53 = call noalias ptr @malloc(i64 noundef %52) #5
  store ptr %53, ptr %7, align 8
  %54 = load i32, ptr @AoS_End, align 4
  %55 = load i32, ptr %2, align 4
  %56 = load ptr, ptr %7, align 8
  call void @populateNodeThree(i32 noundef %55, ptr noundef %56)
  %57 = load ptr, ptr %3, align 8
  call void @free(ptr noundef %57) #6
  %58 = load ptr, ptr %6, align 8
  call void @free(ptr noundef %58) #6
  %59 = load ptr, ptr %7, align 8
  call void @free(ptr noundef %59) #6
  ret i32 0
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #3

; Function Attrs: nounwind
declare void @free(ptr noundef) #4

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind allocsize(0) }
attributes #6 = { nounwind }

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
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
