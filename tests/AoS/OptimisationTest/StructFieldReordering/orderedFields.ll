; ModuleID = 'orderedFields.bc'
source_filename = "unorderedAoS.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.nodeOneOld = type { double, i32, i8, i8 }

@.str = private unnamed_addr constant [16 x i8] c"Validity check\0A\00", align 1
@.str.1 = private unnamed_addr constant [7 x i8] c"a: %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [11 x i8] c"b: %f\0A---\0A\00", align 1
@.str.3 = private unnamed_addr constant [11 x i8] c"c: %c\0A---\0A\00", align 1
@.str.4 = private unnamed_addr constant [11 x i8] c"d: %c\0A---\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeOneOld(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  store i32 0, ptr %5, align 4
  br label %6

6:                                                ; preds = %31, %2
  %7 = load i32, ptr %5, align 4
  %8 = load i32, ptr %4, align 4
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %34

10:                                               ; preds = %6
  %11 = load ptr, ptr %3, align 8
  %12 = load i32, ptr %5, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds %struct.nodeOneOld, ptr %11, i64 %13
  %15 = getelementptr inbounds %struct.nodeOneOld, ptr %14, i32 0, i32 1
  store i32 1, ptr %15, align 8
  %16 = load ptr, ptr %3, align 8
  %17 = load i32, ptr %5, align 4
  %18 = sext i32 %17 to i64
  %19 = getelementptr inbounds %struct.nodeOneOld, ptr %16, i64 %18
  %20 = getelementptr inbounds %struct.nodeOneOld, ptr %19, i32 0, i32 0
  store double 1.000000e+01, ptr %20, align 8
  %21 = load ptr, ptr %3, align 8
  %22 = load i32, ptr %5, align 4
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds %struct.nodeOneOld, ptr %21, i64 %23
  %25 = getelementptr inbounds %struct.nodeOneOld, ptr %24, i32 0, i32 2
  store i8 99, ptr %25, align 8
  %26 = load ptr, ptr %3, align 8
  %27 = load i32, ptr %5, align 4
  %28 = sext i32 %27 to i64
  %29 = getelementptr inbounds %struct.nodeOneOld, ptr %26, i64 %28
  %30 = getelementptr inbounds %struct.nodeOneOld, ptr %29, i32 0, i32 3
  store i8 100, ptr %30, align 1
  br label %31

31:                                               ; preds = %10
  %32 = load i32, ptr %5, align 4
  %33 = add nsw i32 %32, 1
  store i32 %33, ptr %5, align 4
  br label %6, !llvm.loop !6

34:                                               ; preds = %6
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeOneOld(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  store i32 0, ptr %5, align 4
  br label %6

6:                                                ; preds = %52, %2
  %7 = load i32, ptr %5, align 4
  %8 = load i32, ptr %4, align 4
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %55

10:                                               ; preds = %6
  %11 = load ptr, ptr %3, align 8
  %12 = load i32, ptr %5, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds %struct.nodeOneOld, ptr %11, i64 %13
  %15 = getelementptr inbounds %struct.nodeOneOld, ptr %14, i32 0, i32 0
  %16 = load double, ptr %15, align 8
  %17 = load ptr, ptr %3, align 8
  %18 = load i32, ptr %5, align 4
  %19 = sext i32 %18 to i64
  %20 = getelementptr inbounds %struct.nodeOneOld, ptr %17, i64 %19
  %21 = getelementptr inbounds %struct.nodeOneOld, ptr %20, i32 0, i32 1
  %22 = load i32, ptr %21, align 8
  %23 = sitofp i32 %22 to double
  %24 = fmul double %16, %23
  %25 = fdiv double %24, 2.000000e+01
  %26 = fptosi double %25 to i32
  %27 = load ptr, ptr %3, align 8
  %28 = load i32, ptr %5, align 4
  %29 = sext i32 %28 to i64
  %30 = getelementptr inbounds %struct.nodeOneOld, ptr %27, i64 %29
  %31 = getelementptr inbounds %struct.nodeOneOld, ptr %30, i32 0, i32 1
  store i32 %26, ptr %31, align 8
  %32 = load ptr, ptr %3, align 8
  %33 = load i32, ptr %5, align 4
  %34 = sext i32 %33 to i64
  %35 = getelementptr inbounds %struct.nodeOneOld, ptr %32, i64 %34
  %36 = getelementptr inbounds %struct.nodeOneOld, ptr %35, i32 0, i32 1
  %37 = load i32, ptr %36, align 8
  %38 = sitofp i32 %37 to double
  %39 = load ptr, ptr %3, align 8
  %40 = load i32, ptr %5, align 4
  %41 = sext i32 %40 to i64
  %42 = getelementptr inbounds %struct.nodeOneOld, ptr %39, i64 %41
  %43 = getelementptr inbounds %struct.nodeOneOld, ptr %42, i32 0, i32 0
  %44 = load double, ptr %43, align 8
  %45 = fsub double %38, %44
  %46 = fmul double %45, 2.000000e+01
  %47 = load ptr, ptr %3, align 8
  %48 = load i32, ptr %5, align 4
  %49 = sext i32 %48 to i64
  %50 = getelementptr inbounds %struct.nodeOneOld, ptr %47, i64 %49
  %51 = getelementptr inbounds %struct.nodeOneOld, ptr %50, i32 0, i32 0
  store double %46, ptr %51, align 8
  br label %52

52:                                               ; preds = %10
  %53 = load i32, ptr %5, align 4
  %54 = add nsw i32 %53, 1
  store i32 %54, ptr %5, align 4
  br label %6, !llvm.loop !8

55:                                               ; preds = %6
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multArrays(ptr noundef %0, ptr noundef %1, i32 noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store i32 %2, ptr %6, align 4
  store i32 0, ptr %7, align 4
  br label %9

9:                                                ; preds = %58, %3
  %10 = load i32, ptr %7, align 4
  %11 = icmp slt i32 %10, 70
  br i1 %11, label %12, label %61

12:                                               ; preds = %9
  store i32 0, ptr %8, align 4
  br label %13

13:                                               ; preds = %54, %12
  %14 = load i32, ptr %8, align 4
  %15 = load i32, ptr %6, align 4
  %16 = icmp slt i32 %14, %15
  br i1 %16, label %17, label %57

17:                                               ; preds = %13
  %18 = load ptr, ptr %4, align 8
  %19 = load i32, ptr %8, align 4
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds %struct.nodeOneOld, ptr %18, i64 %20
  %22 = getelementptr inbounds %struct.nodeOneOld, ptr %21, i32 0, i32 1
  %23 = load i32, ptr %22, align 8
  %24 = load ptr, ptr %5, align 8
  %25 = load i32, ptr %8, align 4
  %26 = sext i32 %25 to i64
  %27 = getelementptr inbounds %struct.nodeOneOld, ptr %24, i64 %26
  %28 = getelementptr inbounds %struct.nodeOneOld, ptr %27, i32 0, i32 1
  %29 = load i32, ptr %28, align 8
  %30 = mul nsw i32 %23, %29
  %31 = load ptr, ptr %4, align 8
  %32 = load i32, ptr %8, align 4
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds %struct.nodeOneOld, ptr %31, i64 %33
  %35 = getelementptr inbounds %struct.nodeOneOld, ptr %34, i32 0, i32 1
  store i32 %30, ptr %35, align 8
  %36 = load ptr, ptr %4, align 8
  %37 = load i32, ptr %8, align 4
  %38 = sext i32 %37 to i64
  %39 = getelementptr inbounds %struct.nodeOneOld, ptr %36, i64 %38
  %40 = getelementptr inbounds %struct.nodeOneOld, ptr %39, i32 0, i32 0
  %41 = load double, ptr %40, align 8
  %42 = load ptr, ptr %5, align 8
  %43 = load i32, ptr %8, align 4
  %44 = sext i32 %43 to i64
  %45 = getelementptr inbounds %struct.nodeOneOld, ptr %42, i64 %44
  %46 = getelementptr inbounds %struct.nodeOneOld, ptr %45, i32 0, i32 0
  %47 = load double, ptr %46, align 8
  %48 = fmul double %41, %47
  %49 = load ptr, ptr %4, align 8
  %50 = load i32, ptr %8, align 4
  %51 = sext i32 %50 to i64
  %52 = getelementptr inbounds %struct.nodeOneOld, ptr %49, i64 %51
  %53 = getelementptr inbounds %struct.nodeOneOld, ptr %52, i32 0, i32 0
  store double %48, ptr %53, align 8
  br label %54

54:                                               ; preds = %17
  %55 = load i32, ptr %8, align 4
  %56 = add nsw i32 %55, 1
  store i32 %56, ptr %8, align 4
  br label %13, !llvm.loop !9

57:                                               ; preds = %13
  br label %58

58:                                               ; preds = %57
  %59 = load i32, ptr %7, align 4
  %60 = add nsw i32 %59, 1
  store i32 %60, ptr %7, align 4
  br label %9, !llvm.loop !10

61:                                               ; preds = %9
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  store i32 0, ptr %1, align 4
  store i32 999999, ptr %2, align 4
  store i32 0, ptr %3, align 4
  br label %6

6:                                                ; preds = %56, %0
  %7 = load i32, ptr %3, align 4
  %8 = icmp slt i32 %7, 100
  br i1 %8, label %9, label %59

9:                                                ; preds = %6
  %10 = load i32, ptr %2, align 4
  %11 = sext i32 %10 to i64
  %12 = mul i64 %11, 24
  %13 = call noalias ptr @malloc(i64 noundef %12) #4
  store ptr %13, ptr %4, align 8
  %14 = load i32, ptr %2, align 4
  %15 = sext i32 %14 to i64
  %16 = mul i64 %15, 24
  %17 = call noalias ptr @malloc(i64 noundef %16) #4
  store ptr %17, ptr %5, align 8
  %18 = load ptr, ptr %4, align 8
  %19 = load i32, ptr %2, align 4
  call void @populateNodeOneOld(ptr noundef %18, i32 noundef %19)
  %20 = load ptr, ptr %5, align 8
  %21 = load i32, ptr %2, align 4
  call void @populateNodeOneOld(ptr noundef %20, i32 noundef %21)
  %22 = load ptr, ptr %4, align 8
  %23 = load i32, ptr %2, align 4
  call void @multNodeOneOld(ptr noundef %22, i32 noundef %23)
  %24 = load ptr, ptr %4, align 8
  %25 = load ptr, ptr %5, align 8
  %26 = load i32, ptr %2, align 4
  call void @multArrays(ptr noundef %24, ptr noundef %25, i32 noundef %26)
  %27 = load i32, ptr %3, align 4
  %28 = icmp eq i32 %27, 759
  br i1 %28, label %29, label %53

29:                                               ; preds = %9
  %30 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %31 = load ptr, ptr %4, align 8
  %32 = getelementptr inbounds %struct.nodeOneOld, ptr %31, i64 5000
  %33 = getelementptr inbounds %struct.nodeOneOld, ptr %32, i32 0, i32 1
  %34 = load i32, ptr %33, align 8
  %35 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %34)
  %36 = load ptr, ptr %4, align 8
  %37 = getelementptr inbounds %struct.nodeOneOld, ptr %36, i64 5000
  %38 = getelementptr inbounds %struct.nodeOneOld, ptr %37, i32 0, i32 0
  %39 = load double, ptr %38, align 8
  %40 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %39)
  %41 = load ptr, ptr %4, align 8
  %42 = getelementptr inbounds %struct.nodeOneOld, ptr %41, i64 5000
  %43 = getelementptr inbounds %struct.nodeOneOld, ptr %42, i32 0, i32 2
  %44 = load i8, ptr %43, align 8
  %45 = sext i8 %44 to i32
  %46 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %45)
  %47 = load ptr, ptr %4, align 8
  %48 = getelementptr inbounds %struct.nodeOneOld, ptr %47, i64 5000
  %49 = getelementptr inbounds %struct.nodeOneOld, ptr %48, i32 0, i32 3
  %50 = load i8, ptr %49, align 1
  %51 = sext i8 %50 to i32
  %52 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i32 noundef %51)
  br label %53

53:                                               ; preds = %29, %9
  %54 = load ptr, ptr %4, align 8
  call void @free(ptr noundef %54) #5
  %55 = load ptr, ptr %5, align 8
  call void @free(ptr noundef %55) #5
  br label %56

56:                                               ; preds = %53
  %57 = load i32, ptr %3, align 4
  %58 = add nsw i32 %57, 1
  store i32 %58, ptr %3, align 4
  br label %6, !llvm.loop !11

59:                                               ; preds = %6
  ret i32 0
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #1

declare i32 @printf(ptr noundef, ...) #2

; Function Attrs: nounwind
declare void @free(ptr noundef) #3

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
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
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
