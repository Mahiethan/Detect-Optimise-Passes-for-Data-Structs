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
@globalFive = dso_local global ptr null, align 8
@AoS_Start = dso_local global i32 0, align 4
@AoS_End = dso_local global i32 0, align 4
@globalThree = dso_local global i32 0, align 4
@globalFour = dso_local global float 0.000000e+00, align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeOne(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOne, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 0
  store i32 1, ptr %a, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %4 = load i32, ptr %i, align 4
  %inc = add nsw i32 %4, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeTwo(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeTwo, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx, i32 0, i32 0
  store i32 1, ptr %a, align 4
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeTwo, ptr %4, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx2, i32 0, i32 1
  store i8 97, ptr %b, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %6 = load i32, ptr %i, align 4
  %inc = add nsw i32 %6, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !8

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i64 @setNodeTwo() #0 {
entry:
  %retval = alloca %struct.nodeTwo, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr align 4 %retval, ptr align 4 @__const.setNodeTwo.n, i64 8, i1 false)
  %0 = load i64, ptr %retval, align 4
  ret i64 %0
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeThree(i32 noundef %size, ptr noundef %array) #0 {
entry:
  %size.addr = alloca i32, align 4
  %array.addr = alloca ptr, align 8
  %i = alloca i32, align 4
  store i32 %size, ptr %size.addr, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeThree, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeThree, ptr %arrayidx, i32 0, i32 0
  store i32 1, ptr %a, align 8
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeThree, ptr %4, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeThree, ptr %arrayidx2, i32 0, i32 1
  store float 2.000000e+00, ptr %b, align 4
  %6 = load ptr, ptr %array.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %7 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeThree, ptr %6, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeThree, ptr %arrayidx4, i32 0, i32 2
  store double 4.500000e+00, ptr %c, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %8 = load i32, ptr %i, align 4
  %inc = add nsw i32 %8, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !9

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateGlobalOne(i32 noundef %n) #0 {
entry:
  %n.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %n, ptr %n.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %n.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr @globalOne, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOne, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 0
  store i32 1, ptr %a, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %4 = load i32, ptr %i, align 4
  %inc = add nsw i32 %4, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !10

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateGlobalTwo() #0 {
entry:
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %cmp = icmp slt i32 %0, 1293
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %1 = load ptr, ptr @globalTwo, align 8
  %2 = load i32, ptr %i, align 4
  %idxprom = sext i32 %2 to i64
  %arrayidx = getelementptr inbounds %struct.nodeTwo, ptr %1, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx, i32 0, i32 0
  store i32 1, ptr %a, align 4
  %3 = load ptr, ptr @globalTwo, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %4 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeTwo, ptr %3, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx2, i32 0, i32 1
  store i8 98, ptr %b, align 4
  %5 = load ptr, ptr @globalTwo, align 8
  %6 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %6 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeTwo, ptr %5, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx4, i32 0, i32 2
  store i8 97, ptr %c, align 1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %7 = load i32, ptr %i, align 4
  %inc = add nsw i32 %7, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !11

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @printArray(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  call void @populateGlobalTwo()
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load i32, ptr %i, align 4
  %call = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %2)
  %3 = load ptr, ptr %array.addr, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOne, ptr %3, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 0
  %5 = load i32, ptr %a, align 4
  %call1 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %5)
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %6 = load i32, ptr %i, align 4
  %inc = add nsw i32 %6, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !12

for.end:                                          ; preds = %for.cond
  ret void
}

declare i32 @printf(ptr noundef, ...) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populate(ptr noundef %arrayFive, i32 noundef %n) #0 {
entry:
  %arrayFive.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  store ptr %arrayFive, ptr %arrayFive.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  %0 = load i32, ptr %n.addr, align 4
  %1 = load ptr, ptr %arrayFive.addr, align 8
  call void @populateNodeThree(i32 noundef %0, ptr noundef %1)
  %2 = load i32, ptr %n.addr, align 4
  %3 = load ptr, ptr @globalFive, align 8
  call void @populateNodeThree(i32 noundef %2, ptr noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateArrayFive(i32 noundef %a, i32 noundef %n, ptr noundef %arrayFive, i32 noundef %c, i32 noundef %d) #0 {
entry:
  %a.addr = alloca i32, align 4
  %n.addr = alloca i32, align 4
  %arrayFive.addr = alloca ptr, align 8
  %c.addr = alloca i32, align 4
  %d.addr = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  store i32 %n, ptr %n.addr, align 4
  store ptr %arrayFive, ptr %arrayFive.addr, align 8
  store i32 %c, ptr %c.addr, align 4
  store i32 %d, ptr %d.addr, align 4
  %0 = load ptr, ptr @globalOne, align 8
  %1 = load i32, ptr %n.addr, align 4
  call void @printArray(ptr noundef %0, i32 noundef %1)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %n = alloca i32, align 4
  %arrayOne = alloca ptr, align 8
  %testPtr2 = alloca ptr, align 8
  %staticOne = alloca [0 x %struct.nodeOne], align 4
  %staticTwo = alloca [2 x %struct.nodeOne], align 4
  store i32 0, ptr %retval, align 4
  store i32 697000, ptr %n, align 4
  %0 = load i32, ptr @AoS_Start, align 4
  %call = call noalias ptr @malloc(i64 noundef 400) #4
  store ptr %call, ptr %arrayOne, align 8
  %1 = load i32, ptr @AoS_End, align 4
  %2 = load i32, ptr %n, align 4
  %conv = sext i32 %2 to i64
  %mul = mul i64 %conv, 4
  %call1 = call noalias ptr @malloc(i64 noundef %mul) #4
  store ptr %call1, ptr %testPtr2, align 8
  %3 = load ptr, ptr %testPtr2, align 8
  %arrayidx = getelementptr inbounds i32, ptr %3, i64 0
  store i32 102313, ptr %arrayidx, align 4
  %4 = load i32, ptr %n, align 4
  %conv2 = sext i32 %4 to i64
  %mul3 = mul i64 %conv2, 4
  %call4 = call noalias ptr @malloc(i64 noundef %mul3) #4
  store ptr %call4, ptr @globalOne, align 8
  %5 = load i32, ptr %n, align 4
  call void @populateGlobalOne(i32 noundef %5)
  %6 = load i32, ptr %n, align 4
  %7 = load ptr, ptr %arrayOne, align 8
  call void @populateArrayFive(i32 noundef 1, i32 noundef %6, ptr noundef %7, i32 noundef 3, i32 noundef 4)
  ret i32 0
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #3

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind allocsize(0) }

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
