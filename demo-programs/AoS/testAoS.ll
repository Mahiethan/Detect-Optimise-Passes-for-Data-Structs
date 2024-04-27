; ModuleID = 'testAoS.c'
source_filename = "testAoS.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.nodeOne = type { i32, i8, double }
%struct.nodeTwo = type { float, double }

@globalFive = dso_local global [100 x %struct.nodeOne] zeroinitializer, align 16
@globalSix = dso_local global ptr null, align 8
@globalOne = dso_local global [100 x %struct.nodeOne] zeroinitializer, align 16
@globalTwo = dso_local global [100 x %struct.nodeTwo] zeroinitializer, align 16
@globalThree = dso_local global [100 x %struct.nodeOne] zeroinitializer, align 16
@globalFour = dso_local global [100 x %struct.nodeTwo] zeroinitializer, align 16
@globalSeven = dso_local global ptr null, align 8

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @pop() #0 {
entry:
  store double 0.000000e+00, ptr getelementptr inbounds (%struct.nodeOne, ptr @globalFive, i32 0, i32 2), align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateTwo(ptr noundef %ptr) #0 {
entry:
  %ptr.addr = alloca ptr, align 8
  store ptr %ptr, ptr %ptr.addr, align 8
  %0 = load ptr, ptr %ptr.addr, align 8
  %arrayidx = getelementptr inbounds %struct.nodeOne, ptr %0, i64 10
  %b = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 1
  store i8 99, ptr %b, align 4
  call void @pop()
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateOne(ptr noundef %ptr) #0 {
entry:
  %ptr.addr = alloca ptr, align 8
  %tempOne = alloca ptr, align 8
  store ptr %ptr, ptr %ptr.addr, align 8
  %call = call noalias ptr @malloc(i64 noundef 1600) #2
  store ptr %call, ptr %tempOne, align 8
  %0 = load ptr, ptr %tempOne, align 8
  %arrayidx = getelementptr inbounds %struct.nodeOne, ptr %0, i64 0
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 0
  store i32 10, ptr %a, align 8
  %1 = load ptr, ptr %ptr.addr, align 8
  call void @populateTwo(ptr noundef %1)
  %2 = load ptr, ptr @globalSix, align 8
  %arrayidx1 = getelementptr inbounds %struct.nodeOne, ptr %2, i64 9
  %a2 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx1, i32 0, i32 0
  store i32 100, ptr %a2, align 8
  ret void
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %arrayOne = alloca ptr, align 8
  %arrayTwo = alloca ptr, align 8
  %staticOne = alloca [1000 x %struct.nodeOne], align 16
  store i32 0, ptr %retval, align 4
  %call = call noalias ptr @malloc(i64 noundef 1600) #2
  store ptr %call, ptr %arrayOne, align 8
  %call1 = call noalias ptr @malloc(i64 noundef 1600) #2
  store ptr %call1, ptr %arrayTwo, align 8
  %0 = load ptr, ptr %arrayOne, align 8
  store ptr %0, ptr @globalSix, align 8
  %1 = load ptr, ptr %arrayOne, align 8
  call void @populateOne(ptr noundef %1)
  %2 = load ptr, ptr %arrayTwo, align 8
  call void @populateOne(ptr noundef %2)
  %3 = load ptr, ptr @globalSix, align 8
  call void @populateOne(ptr noundef %3)
  %arraydecay = getelementptr inbounds [1000 x %struct.nodeOne], ptr %staticOne, i64 0, i64 0
  call void @populateTwo(ptr noundef %arraydecay)
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind allocsize(0) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.0.0 (https://github.com/llvm/llvm-project.git 893416051d517d979481510fe3b52d83ba0d1e01)"}
