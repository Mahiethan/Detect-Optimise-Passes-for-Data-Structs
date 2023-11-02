; ModuleID = 'dynamic_AoS.c'
source_filename = "dynamic_AoS.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.nodeOne = type { i32 }
%struct.nodeTwo = type { i32, i8, i8 }
%struct.nodeThree = type { i32, float, double }

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
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca %struct.nodeOne, align 4
  %4 = alloca %struct.nodeTwo, align 4
  %5 = alloca %struct.nodeThree, align 8
  %6 = alloca ptr, align 8
  %7 = alloca ptr, align 8
  %8 = alloca ptr, align 8
  %9 = alloca ptr, align 8
  store i32 0, ptr %1, align 4
  store i32 697000, ptr %2, align 4
  %10 = load i32, ptr @AoS_Start, align 4
  %11 = load i32, ptr %2, align 4
  %12 = sext i32 %11 to i64
  %13 = mul i64 %12, 4
  %14 = call noalias ptr @malloc(i64 noundef %13) #3
  store ptr %14, ptr %6, align 8
  %15 = load i32, ptr @AoS_End, align 4
  %16 = load i32, ptr %2, align 4
  %17 = sext i32 %16 to i64
  %18 = mul i64 %17, 4
  %19 = call noalias ptr @malloc(i64 noundef %18) #3
  store ptr %19, ptr %7, align 8
  %20 = load ptr, ptr %6, align 8
  %21 = load i32, ptr %2, align 4
  call void @populateNodeOne(ptr noundef %20, i32 noundef %21)
  store i32 7080, ptr %2, align 4
  %22 = load i32, ptr @AoS_Start, align 4
  %23 = load i32, ptr %2, align 4
  %24 = sext i32 %23 to i64
  %25 = mul i64 %24, 8
  %26 = call noalias ptr @malloc(i64 noundef %25) #3
  store ptr %26, ptr %8, align 8
  %27 = load i32, ptr @AoS_End, align 4
  store i32 372, ptr %2, align 4
  %28 = load i32, ptr @AoS_Start, align 4
  %29 = load i32, ptr %2, align 4
  %30 = sext i32 %29 to i64
  %31 = mul i64 %30, 16
  %32 = call noalias ptr @malloc(i64 noundef %31) #3
  store ptr %32, ptr %9, align 8
  %33 = load i32, ptr @AoS_End, align 4
  %34 = load ptr, ptr %6, align 8
  call void @free(ptr noundef %34) #4
  %35 = load ptr, ptr %8, align 8
  call void @free(ptr noundef %35) #4
  %36 = load ptr, ptr %9, align 8
  call void @free(ptr noundef %36) #4
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
