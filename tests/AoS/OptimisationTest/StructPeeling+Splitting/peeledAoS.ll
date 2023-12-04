; ModuleID = 'peeledAoS.c'
source_filename = "peeledAoS.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.nodeOneHot = type { i32, double }
%struct.nodeOneCold = type { i32, double }

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeOneOld(ptr noundef %0, ptr noundef %1, i32 noundef %2) #0 {
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

9:                                                ; preds = %24, %3
  %10 = load i32, ptr %7, align 4
  %11 = load i32, ptr %6, align 4
  %12 = icmp slt i32 %10, %11
  br i1 %12, label %13, label %27

13:                                               ; preds = %9
  %14 = load ptr, ptr %4, align 8
  %15 = load i32, ptr %7, align 4
  %16 = sext i32 %15 to i64
  %17 = getelementptr inbounds %struct.nodeOneHot, ptr %14, i64 %16
  %18 = getelementptr inbounds %struct.nodeOneHot, ptr %17, i32 0, i32 0
  store i32 1, ptr %18, align 8
  %19 = load ptr, ptr %4, align 8
  %20 = load i32, ptr %7, align 4
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds %struct.nodeOneHot, ptr %19, i64 %21
  %23 = getelementptr inbounds %struct.nodeOneHot, ptr %22, i32 0, i32 1
  store double 1.000000e+01, ptr %23, align 8
  br label %24

24:                                               ; preds = %13
  %25 = load i32, ptr %7, align 4
  %26 = add nsw i32 %25, 1
  store i32 %26, ptr %7, align 4
  br label %9, !llvm.loop !6

27:                                               ; preds = %9
  store i32 0, ptr %8, align 4
  br label %28

28:                                               ; preds = %43, %27
  %29 = load i32, ptr %8, align 4
  %30 = load i32, ptr %6, align 4
  %31 = icmp slt i32 %29, %30
  br i1 %31, label %32, label %46

32:                                               ; preds = %28
  %33 = load ptr, ptr %5, align 8
  %34 = load i32, ptr %8, align 4
  %35 = sext i32 %34 to i64
  %36 = getelementptr inbounds %struct.nodeOneCold, ptr %33, i64 %35
  %37 = getelementptr inbounds %struct.nodeOneCold, ptr %36, i32 0, i32 0
  store i32 99, ptr %37, align 8
  %38 = load ptr, ptr %5, align 8
  %39 = load i32, ptr %8, align 4
  %40 = sext i32 %39 to i64
  %41 = getelementptr inbounds %struct.nodeOneCold, ptr %38, i64 %40
  %42 = getelementptr inbounds %struct.nodeOneCold, ptr %41, i32 0, i32 1
  store double 2.300000e+01, ptr %42, align 8
  br label %43

43:                                               ; preds = %32
  %44 = load i32, ptr %8, align 4
  %45 = add nsw i32 %44, 1
  store i32 %45, ptr %8, align 4
  br label %28, !llvm.loop !8

46:                                               ; preds = %28
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeOneOld(ptr noundef %0, ptr noundef %1, i32 noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store i32 %2, ptr %6, align 4
  store i32 0, ptr %7, align 4
  br label %8

8:                                                ; preds = %54, %3
  %9 = load i32, ptr %7, align 4
  %10 = load i32, ptr %6, align 4
  %11 = icmp slt i32 %9, %10
  br i1 %11, label %12, label %57

12:                                               ; preds = %8
  %13 = load ptr, ptr %4, align 8
  %14 = load i32, ptr %7, align 4
  %15 = sext i32 %14 to i64
  %16 = getelementptr inbounds %struct.nodeOneHot, ptr %13, i64 %15
  %17 = getelementptr inbounds %struct.nodeOneHot, ptr %16, i32 0, i32 1
  %18 = load double, ptr %17, align 8
  %19 = load ptr, ptr %4, align 8
  %20 = load i32, ptr %7, align 4
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds %struct.nodeOneHot, ptr %19, i64 %21
  %23 = getelementptr inbounds %struct.nodeOneHot, ptr %22, i32 0, i32 0
  %24 = load i32, ptr %23, align 8
  %25 = sitofp i32 %24 to double
  %26 = fmul double %18, %25
  %27 = fdiv double %26, 2.000000e+01
  %28 = fptosi double %27 to i32
  %29 = load ptr, ptr %4, align 8
  %30 = load i32, ptr %7, align 4
  %31 = sext i32 %30 to i64
  %32 = getelementptr inbounds %struct.nodeOneHot, ptr %29, i64 %31
  %33 = getelementptr inbounds %struct.nodeOneHot, ptr %32, i32 0, i32 0
  store i32 %28, ptr %33, align 8
  %34 = load ptr, ptr %4, align 8
  %35 = load i32, ptr %7, align 4
  %36 = sext i32 %35 to i64
  %37 = getelementptr inbounds %struct.nodeOneHot, ptr %34, i64 %36
  %38 = getelementptr inbounds %struct.nodeOneHot, ptr %37, i32 0, i32 0
  %39 = load i32, ptr %38, align 8
  %40 = sitofp i32 %39 to double
  %41 = load ptr, ptr %4, align 8
  %42 = load i32, ptr %7, align 4
  %43 = sext i32 %42 to i64
  %44 = getelementptr inbounds %struct.nodeOneHot, ptr %41, i64 %43
  %45 = getelementptr inbounds %struct.nodeOneHot, ptr %44, i32 0, i32 1
  %46 = load double, ptr %45, align 8
  %47 = fdiv double %40, %46
  %48 = fdiv double %47, 2.970000e+02
  %49 = load ptr, ptr %4, align 8
  %50 = load i32, ptr %7, align 4
  %51 = sext i32 %50 to i64
  %52 = getelementptr inbounds %struct.nodeOneHot, ptr %49, i64 %51
  %53 = getelementptr inbounds %struct.nodeOneHot, ptr %52, i32 0, i32 1
  store double %48, ptr %53, align 8
  br label %54

54:                                               ; preds = %12
  %55 = load i32, ptr %7, align 4
  %56 = add nsw i32 %55, 1
  store i32 %56, ptr %7, align 4
  br label %8, !llvm.loop !9

57:                                               ; preds = %8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multArrays(ptr noundef %0, ptr noundef %1, i32 noundef %2) #0 {
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  store i32 %2, ptr %6, align 4
  store i32 0, ptr %7, align 4
  br label %8

8:                                                ; preds = %49, %3
  %9 = load i32, ptr %7, align 4
  %10 = load i32, ptr %6, align 4
  %11 = icmp slt i32 %9, %10
  br i1 %11, label %12, label %52

12:                                               ; preds = %8
  %13 = load ptr, ptr %4, align 8
  %14 = load i32, ptr %7, align 4
  %15 = sext i32 %14 to i64
  %16 = getelementptr inbounds %struct.nodeOneHot, ptr %13, i64 %15
  %17 = getelementptr inbounds %struct.nodeOneHot, ptr %16, i32 0, i32 0
  %18 = load i32, ptr %17, align 8
  %19 = load ptr, ptr %5, align 8
  %20 = load i32, ptr %7, align 4
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds %struct.nodeOneHot, ptr %19, i64 %21
  %23 = getelementptr inbounds %struct.nodeOneHot, ptr %22, i32 0, i32 0
  %24 = load i32, ptr %23, align 8
  %25 = mul nsw i32 %18, %24
  %26 = load ptr, ptr %4, align 8
  %27 = load i32, ptr %7, align 4
  %28 = sext i32 %27 to i64
  %29 = getelementptr inbounds %struct.nodeOneHot, ptr %26, i64 %28
  %30 = getelementptr inbounds %struct.nodeOneHot, ptr %29, i32 0, i32 0
  store i32 %25, ptr %30, align 8
  %31 = load ptr, ptr %4, align 8
  %32 = load i32, ptr %7, align 4
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds %struct.nodeOneHot, ptr %31, i64 %33
  %35 = getelementptr inbounds %struct.nodeOneHot, ptr %34, i32 0, i32 1
  %36 = load double, ptr %35, align 8
  %37 = load ptr, ptr %5, align 8
  %38 = load i32, ptr %7, align 4
  %39 = sext i32 %38 to i64
  %40 = getelementptr inbounds %struct.nodeOneHot, ptr %37, i64 %39
  %41 = getelementptr inbounds %struct.nodeOneHot, ptr %40, i32 0, i32 1
  %42 = load double, ptr %41, align 8
  %43 = fmul double %36, %42
  %44 = load ptr, ptr %4, align 8
  %45 = load i32, ptr %7, align 4
  %46 = sext i32 %45 to i64
  %47 = getelementptr inbounds %struct.nodeOneHot, ptr %44, i64 %46
  %48 = getelementptr inbounds %struct.nodeOneHot, ptr %47, i32 0, i32 1
  store double %43, ptr %48, align 8
  br label %49

49:                                               ; preds = %12
  %50 = load i32, ptr %7, align 4
  %51 = add nsw i32 %50, 1
  store i32 %51, ptr %7, align 4
  br label %8, !llvm.loop !10

52:                                               ; preds = %8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca ptr, align 8
  store i32 0, ptr %1, align 4
  store i32 999999, ptr %2, align 4
  store i32 0, ptr %3, align 4
  br label %8

8:                                                ; preds = %38, %0
  %9 = load i32, ptr %3, align 4
  %10 = icmp slt i32 %9, 100
  br i1 %10, label %11, label %41

11:                                               ; preds = %8
  %12 = load i32, ptr %2, align 4
  %13 = sext i32 %12 to i64
  %14 = mul i64 %13, 16
  %15 = call noalias ptr @malloc(i64 noundef %14) #3
  store ptr %15, ptr %4, align 8
  %16 = load i32, ptr %2, align 4
  %17 = sext i32 %16 to i64
  %18 = mul i64 %17, 16
  %19 = call noalias ptr @malloc(i64 noundef %18) #3
  store ptr %19, ptr %5, align 8
  %20 = load i32, ptr %2, align 4
  %21 = sext i32 %20 to i64
  %22 = mul i64 %21, 16
  %23 = call noalias ptr @malloc(i64 noundef %22) #3
  store ptr %23, ptr %6, align 8
  %24 = load i32, ptr %2, align 4
  %25 = sext i32 %24 to i64
  %26 = mul i64 %25, 16
  %27 = call noalias ptr @malloc(i64 noundef %26) #3
  store ptr %27, ptr %7, align 8
  %28 = load ptr, ptr %4, align 8
  %29 = load ptr, ptr %5, align 8
  %30 = load i32, ptr %2, align 4
  call void @populateNodeOneOld(ptr noundef %28, ptr noundef %29, i32 noundef %30)
  %31 = load ptr, ptr %6, align 8
  %32 = load ptr, ptr %7, align 8
  %33 = load i32, ptr %2, align 4
  call void @populateNodeOneOld(ptr noundef %31, ptr noundef %32, i32 noundef %33)
  %34 = load ptr, ptr %4, align 8
  call void @free(ptr noundef %34) #4
  %35 = load ptr, ptr %5, align 8
  call void @free(ptr noundef %35) #4
  %36 = load ptr, ptr %6, align 8
  call void @free(ptr noundef %36) #4
  %37 = load ptr, ptr %7, align 8
  call void @free(ptr noundef %37) #4
  br label %38

38:                                               ; preds = %11
  %39 = load i32, ptr %3, align 4
  %40 = add nsw i32 %39, 1
  store i32 %40, ptr %3, align 4
  br label %8, !llvm.loop !11

41:                                               ; preds = %8
  ret i32 0
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #1

; Function Attrs: nounwind
declare void @free(ptr noundef) #2

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind allocsize(0) }
attributes #4 = { nounwind }

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
